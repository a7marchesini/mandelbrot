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

extern IterationColors iteration_colors;

int main() {

  InitWindow(800, 600, "Mandelbrot");

  if (!IsWindowReady()) {
    CloseWindow();
    return 1;
  }

  SetTargetFPS(60);

  SetWindowState(FLAG_VSYNC_HINT);
  SetWindowState(FLAG_WINDOW_RESIZABLE);

  //============
  // USER INPUTS
  //============

  // Pan and zoom speeds
  double pan_speed = 0.01;
  double zoom_speed = 0.01;

  // Calculation choice to use
  int calc_choice = 1;

  // Iterations for determinations
  int max_iterations = 32;

  //===========
  // IMAGE DATA
  //===========

  // Colors that form the image
  Colors colors;

  // Image and texture definitions
  Image image;
  Texture2D texture;

  //==========
  // AXES DATA
  //==========

  // What should we be plotting?
  Eigen::Vector2d x_axis = {-2.5, 1};
  Eigen::Vector2d y_axis = {-1, 1};

  // X and Y axis locations
  EIGEN_VECTOR x_pixels_pos;
  EIGEN_VECTOR y_pixels_pos;

  //================
  // CALCULATED DATA
  //================

  // Calculated norms and iterations
  EIGEN_MATRIX norms;
  IterationCounts iteration_counts;
  
  //============
  // THREAD DATA
  //============

  // Set up function for threads
  std::vector<std::thread> threads;
  threads.resize(NUM_THREADS);

  // Set up row for threads
  int y_next;
  std::mutex y_next_mut;

  // Set up semaphores for threads
  std::counting_semaphore<INT_MAX> semaphore_main_to_thread(0);
  std::counting_semaphore<INT_MAX> semaphore_thread_to_main(0);

  // Set up exit check for threads
  bool thread_exit_flag = false;

  // Create threads
  for (size_t thread = 0; thread < threads.size(); ++thread)
  {
    threads[thread] = std::thread(
      thread_fn,
      &max_iterations,
      &calc_choice,
      &x_pixels_pos,
      &y_pixels_pos,
      &norms,
      &iteration_counts,
      &y_next,
      &y_next_mut,
      &semaphore_main_to_thread,
      &semaphore_thread_to_main,
      &thread_exit_flag);
  }
  
  //==========
  // MAIN LOOP
  //==========

  // Initialize once
  reinit(&colors, &image, &texture, &x_pixels_pos, &y_pixels_pos, &norms, &iteration_counts);

  while(!WindowShouldClose())
  {
    // Handle user input
    user_input(&x_axis, &y_axis, &pan_speed, &zoom_speed, &x_pixels_pos, &y_pixels_pos, &max_iterations, &calc_choice);

    if (IsWindowResized())
    {
      reinit(&colors, &image, &texture, &x_pixels_pos, &y_pixels_pos, &norms, &iteration_counts);
    }

    BeginDrawing();

    ClearBackground(BLUE);

    // Reset row to process next
    y_next = 0;

    // Release threads
    for (size_t thread = 0; thread < threads.size(); ++thread) {
      semaphore_main_to_thread.release();
    }

    // Wait for threads
    for (size_t thread = 0; thread < threads.size(); ++thread) {
      semaphore_thread_to_main.acquire();
    }

    // For each pixel, load the appropriate color
    for (int y = 0; y < colors.rows(); ++y)
    {
      for (int x = 0; x < colors.cols(); ++x)
      {
        // Escape velocity sort of?
        const VALUE_TYPE norm = norms(y, x);
        double iteration = iteration_counts(y, x);
        if (iteration == max_iterations)
        {
          colors(y, x) = BLACK;
        }
        else
        {
          int iteration = iteration_counts(y, x);
          colors(y, x) = iteration_colors[iteration % MAX_ITERATION_COLORS];

          // Try histogram method
          //hues(y, x) = hist(Eigen::seq(0, iteration + 1)).sum() * 1.0 / (GetScreenHeight() * GetScreenWidth());

          //double hue = hues(y, x);
          ////std::cout << hue << std::endl;
          //colors(y, x) = iteration_colors[int(hue * MAX_ITERATION_COLORS)];

          //// Try smooth coloring
          //double log_zn = std::log(norm) / 2;
          //double nu = std::log2(log_zn / std::log(2));
          //iteration = std::max(iteration + 1 - nu, 0.0);

          //// Interpolate?
          //Color color1 = iteration_colors[int(iteration)];
          //Color color2 = iteration_colors[int(iteration) + 1];
          //double fraction = std::fmod(iteration, 1.0);
          //Color interpolated_color = Color{ uint8_t(color1.r + (color2.r - color1.r) * fraction),
          //uint8_t(color1.g + (color2.g - color1.g) * fraction),
          //uint8_t(color1.b + (color2.b - color1.b) * fraction), 255};
          //colors(y, x) = interpolated_color;
        }
      }
    }

    // Draw colors
    UpdateTexture(texture, colors.data());
    DrawTexture(texture, 0, 0, WHITE);

    DrawFPS(10, 10);

    // Draw text
    char text[256];
    snprintf(text, sizeof(text) - 1, "Iterations: %d", max_iterations);
    DrawText(text, 10, 30, 20, WHITE);

    // Draw calcs choice
    if (calc_choice == 1)
    {
      snprintf(text, sizeof(text) - 1, "Calcs choice: scalar");
    }
    else if (calc_choice == 2)
    {
      snprintf(text, sizeof(text) - 1, "Calcs choice: SIMD");
    }
    DrawText(text, 10, 50, 20, WHITE);
    
    // Draw pan and zoom speed choice
    snprintf(text, sizeof(text) - 1, "Pan speed: %.3f Zoom speed: %.3f", pan_speed, zoom_speed);
    DrawText(text, 10, 70, 20, WHITE);
    
    // Draw axes limits
    snprintf(text, sizeof(text) - 1, "Boundaries: x [%5.3G, %5.3G], y [%5.3G, %5.3G]", x_axis[0], x_axis[1], y_axis[0], y_axis[1]);
    DrawText(text, 10, 90, 20, WHITE);

    EndDrawing();
  }

  CloseWindow();

  // Kill threads
  thread_exit_flag = true;
  for (size_t thread = 0; thread < threads.size(); ++thread) {
    semaphore_main_to_thread.release();
  }

  return 0;
}

