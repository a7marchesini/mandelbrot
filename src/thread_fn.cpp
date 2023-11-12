
#include "defines_and_types.hpp"
#include "fn_defs.hpp"

#include <mutex>
#include <iostream>
#include <semaphore>

extern IterationColors iteration_colors;

void thread_fn(
  int *p_max_iterations,
  int *p_calc_choice,
  EIGEN_VECTOR *p_x_pixels_pos,
  EIGEN_VECTOR *p_y_pixels_pos,
  EIGEN_MATRIX *p_norms,
  IterationCounts *p_iteration_counts,
  int *p_y_next,
  std::mutex *p_y_next_mut,
  std::counting_semaphore<INT_MAX> *p_semaphore_main_to_thread,
  std::counting_semaphore<INT_MAX> *p_semaphore_thread_to_main,
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

    // Process rows while we can
    for (;;)
    {
      // Try to grab a new row
      int y;
      {
        std::lock_guard<std::mutex> lock(*p_y_next_mut);
        if (*p_y_next != p_y_pixels_pos->size())
        {
          y = *p_y_next;
          ++*p_y_next;
        }
        // If there are no rows left to process, exit the loop
        else
        {
          break;
        }
      }
      
      if (*p_calc_choice == 1)
      {
      calcs_scalar(*p_max_iterations, y, p_x_pixels_pos, p_y_pixels_pos, p_norms, p_iteration_counts);
      }
      else if (*p_calc_choice == 2)
      {
      calcs_simd(*p_max_iterations, y, p_x_pixels_pos, p_y_pixels_pos, p_norms, p_iteration_counts);
      }
    }

    // Notify main thread
    p_semaphore_thread_to_main->release();
  }
};