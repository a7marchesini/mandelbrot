
#include "defines_and_types.hpp"
#include "fn_defs.hpp"

#include <semaphore>

extern IterationColors iteration_colors;

void thread_fn(
  int *p_y_first,
  int *p_y_last,
  EIGEN_VECTOR *p_x_pixels_pos,
  EIGEN_VECTOR *p_y_pixels_pos,
  int *p_calc_choice,
  int *p_iterations,
  Colors *p_colors,
  std::binary_semaphore *p_semaphore_main_to_thread,
  std::binary_semaphore *p_semaphore_thread_to_main,
  const bool *p_exit_flag)
{

  for (;;) {

    // Wait until told to execute
    p_semaphore_main_to_thread->acquire();

    // Exit if program ends
    if (*p_exit_flag)
    {
      return;
    }

    // Figure out the last indices we can use before we need to switch to the remainder loop
    int x_last_aligned = GetScreenWidth() - (GetScreenWidth() % VALUES_PER_REGISTER);

    // Iterate through lines
    for (int y = *p_y_first; y < *p_y_last; ++y) {
      // Iterate through columns
      for (int x = 0; x < x_last_aligned; x += VALUES_PER_REGISTER)
      {
        if (y == 160 && x == 428)
        {
          int a = 3;
          ++a;
        }

        // Prepare registers
        const SIMD_REGISTER c_r = SIMD_LOAD(p_x_pixels_pos->data() + x);
        const SIMD_REGISTER c_i =  SIMD_SET_ONE((*p_y_pixels_pos)[y]);

        // Calculate iterations
        CalculateIterationsData data;
        if (*p_calc_choice == 0)
        {
          data = calcs_scalar(*p_iterations, c_r, c_i);
        }
        else if (*p_calc_choice == 1)
        {
          data = calcs_simd(*p_iterations, c_r, c_i);
        }

        // For each pixel, load the appropriate color
        for (int i = 0; i < VALUES_PER_REGISTER; ++i)
        {
          if (data.norm_[i] < 4.0)
          {
            (*p_colors)(y, x + i) = BLACK;
          }
          else
          {
            int iteration = int(data.iteration_counts_[i]);
            (*p_colors)(y, x + i) = iteration_colors[iteration % MAX_ITERATION_COLORS];
          }
        }
      }
    }

    // Notify main thread
    p_semaphore_thread_to_main->release();
  }
};