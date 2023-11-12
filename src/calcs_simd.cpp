#include "defines_and_types.hpp"

void calcs_simd(int max_iterations, int y, EIGEN_VECTOR *p_x_pixels_pos, EIGEN_VECTOR *p_y_pixels_pos, EIGEN_MATRIX *p_norms, IterationCounts *p_iteration_counts)
{
  double nan = NAN;

  const SIMD_REGISTER threshold = SIMD_SET_ONE(sqrt(5.0));

  // Iterate through columns
  for (int x = 0; x < p_x_pixels_pos->size(); x += VALUES_PER_REGISTER)
  {
    const SIMD_REGISTER c_r = SIMD_LOAD(&(*p_x_pixels_pos)(x));
    const SIMD_REGISTER c_i = SIMD_SET_ONE((*p_y_pixels_pos)(y));
    SIMD_REGISTER value_r = SIMD_SET_ZERO();
    SIMD_REGISTER value_i = SIMD_SET_ZERO();
    SIMD_REGISTER norm = SIMD_SET_ZERO();
    SIMD_INT_REGISTER iteration_counts = SIMD_INT_SET_ONE(0);
    // Run the iterations
    for (int iteration = 0; iteration < max_iterations; ++iteration) {
      // Square ourselves and add c
      SIMD_REGISTER a = value_r;
      SIMD_REGISTER b = value_i;
      value_r = SIMD_SUB(SIMD_FMADD(a, a, c_r), SIMD_MUL(b, b));
      value_i = SIMD_FMADD(SIMD_MUL(SIMD_SET_ONE(2.0), a), b, c_i);
      // Compare the norms against the threshold
      norm =  SIMD_FMADD(value_r, value_r, SIMD_MUL(value_i, value_i));
      const SIMD_REGISTER norm_lt_threshold = SIMD_CMP(norm, threshold, _CMP_LE_OS);
      // Mask out the data above the threshold and increase the thresholds
      iteration_counts = SIMD_INT_ADD(iteration_counts, SIMD_VALUE_TO_INT(SIMD_AND(SIMD_SET_ONE(1.0), norm_lt_threshold)));
    }
    // Populate data containers
    SIMD_STORE(&(*p_norms)(y, x), norm);
    SIMD_INT_LOAD(&(*p_iteration_counts)(y, x), SIMD_INT_SET_ONE(0xFFFFFFFF), iteration_counts);
  }
};
