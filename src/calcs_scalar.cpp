#include "defines_and_types.hpp"

// Create lambda to calculate iterations
//void calcs_scalar(int max_iterations, int y, EIGEN_VECTOR *p_x_pixels_pos, EIGEN_VECTOR *p_y_pixels_pos, EIGEN_MATRIX *p_norms, IterationCounts *p_iteration_counts)
//{
//  double nan = NAN;
//
//  // Iterate through columns
//  for (int x = 0; x < p_x_pixels_pos->size(); ++x)
//  {
//    const VALUE_TYPE c_r = (*p_x_pixels_pos)(x);
//    const VALUE_TYPE c_i = (*p_y_pixels_pos)(y);
//    VALUE_TYPE value_r = 0;
//    VALUE_TYPE value_i = 0;
//    VALUE_TYPE norm = 0;
//    int iteration_count = 0;
//    // Run the iterations
//    for (int iteration = 0; iteration < max_iterations; ++iteration) {
//      // Square ourselves and add c
//      VALUE_TYPE a = value_r;
//      VALUE_TYPE b = value_i;
//      value_r = a * a - b * b + c_r;
//      value_i = VALUE_TYPE(2.0) * a * b + c_i;
//      // Compare the norms against the threshold
//      norm = value_r * value_r + value_i * value_i;
//      const VALUE_TYPE norm_lt_threshold = norm < VALUE_TYPE(sqrt(5.0));
//      // Mask out the data above the threshold and increase the thresholds
//      iteration_count += norm_lt_threshold;
//    }
//    // Fix inf and nan
//    if (isnan(norm) || isinf(norm))
//    {
//      norm = FLT_MAX;
//    }
//    // Populate data containers
//    (*p_norms)(y, x) = norm;
//    (*p_iteration_counts)(y, x) = iteration_count;
//  }
//};

void calcs_scalar(int max_iterations, int y, EIGEN_VECTOR *p_x_pixels_pos, EIGEN_VECTOR *p_y_pixels_pos, EIGEN_MATRIX *p_norms, IterationCounts *p_iteration_counts)
{
  double nan = NAN;

  const VALUE_TYPE threshold(sqrt(5.0));

  // Iterate through columns
  for (int x = 0; x < p_x_pixels_pos->size(); ++x)
  {
    const VALUE_TYPE c_r = (*p_x_pixels_pos)(x);
    const VALUE_TYPE c_i = (*p_y_pixels_pos)(y);
    VALUE_TYPE value_r = 0;
    VALUE_TYPE value_i = 0;
    VALUE_TYPE norm = 0;
    int iteration_count = 0;
    // Run the iterations
    for (int iteration = 0; iteration < max_iterations; ++iteration) {
      // Square ourselves and add c
      VALUE_TYPE a = value_r;
      VALUE_TYPE b = value_i;
      value_r = a * a - b * b + c_r;
      value_i = VALUE_TYPE(2.0) * a * b + c_i;
      // Compare the norms against the threshold
      norm = value_r * value_r + value_i * value_i;
      const VALUE_TYPE norm_lt_threshold = norm < threshold;
      // Mask out the data above the threshold and increase the thresholds
      iteration_count += norm_lt_threshold;
    }
    // Populate data containers
    (*p_norms)(y, x) = norm;
    (*p_iteration_counts)(y, x) = iteration_count;
  }
};
