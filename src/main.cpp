//
// Created by Alessandro on 9/19/2023.
//

#include <Eigen/Dense>
#include <raylib.h>

#include <mutex>
#include <thread>
#include <semaphore>
#include <iostream>

#include <immintrin.h>

#include "defines_and_types.hpp"
#include "fn_defs.hpp"


int main() {

  InitWindow(800, 600, "Mandelbrot");

  if (!IsWindowReady()) {
    CloseWindow();
    return 1;
  }

  SetTargetFPS(60);

  SetWindowState(FLAG_WINDOW_RESIZABLE);

  // GetScreenHeight

  // What should we be plotting?
  Eigen::Vector2d x_axis = {-2, 1.5};
  Eigen::Vector2d y_axis = {-1.5, 1.5};

  // Pan and zoom speeds
  double pan_speed = 0.01;
  double zoom_speed = 0.01;

  // Calculation choice to use
  int calc_choice = 1;

  // Iterations for determinations
  int iterations = 32;

  // Colors that form the image
  Colors colors;

  // Image and texture definitions
  Image image;
  Texture2D texture;

  // X and Y axis locations
  EIGEN_VECTOR x_pixels_pos;
  EIGEN_VECTOR y_pixels_pos;

  // Y pixel ranges for each thread
  Eigen::VectorXi thread_y_pixel_ranges(NUM_THREADS + 1);

  // Set up function for threads
  std::vector<std::thread> threads;
  threads.resize(NUM_THREADS);

  // Set up semaphores for threads
  std::binary_semaphore semaphore_main_to_thread(0);
  std::binary_semaphore semaphore_thread_to_main(0);

  // Set up exit check for threads
  bool thread_exit_flag = false;

  // Create threads
  for (size_t thread = 0; thread < threads.size(); ++thread)
  {
    threads[thread] = std::thread(
      thread_fn,
      &thread_y_pixel_ranges[thread],
      &thread_y_pixel_ranges[thread + 1],
      &x_pixels_pos,
      &y_pixels_pos,
      &calc_choice,
      &iterations,
      &colors,
      &semaphore_main_to_thread,
      &semaphore_thread_to_main,
      &thread_exit_flag);
  }

  // Initialize once
  reinit(&x_pixels_pos, &y_pixels_pos, &thread_y_pixel_ranges, &colors, &image, &texture);

  while(!WindowShouldClose())
  {
    // Handle user input
    user_input(&x_axis, &y_axis, &pan_speed, &zoom_speed, &x_pixels_pos, &y_pixels_pos, &iterations);

    if (IsWindowResized())
    {
      reinit(&x_pixels_pos, &y_pixels_pos,&thread_y_pixel_ranges, &colors, &image, &texture);
    }

    BeginDrawing();

    ClearBackground(BLUE);

    // Release threads
    semaphore_main_to_thread.release(threads.size());

    // Wait for threads
    for (size_t thread = 0; thread < threads.size(); ++thread) {
      semaphore_thread_to_main.acquire();
    }

    // Draw colors
    UpdateTexture(texture, colors.data());
    DrawTexture(texture, 0, 0, WHITE);

    DrawFPS(10, 10);

    char iterations_text[256];
    snprintf(iterations_text, sizeof(iterations_text) - 1, "Iterations: %d", iterations);
    DrawText(iterations_text, 10, 30, 32, WHITE);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}

