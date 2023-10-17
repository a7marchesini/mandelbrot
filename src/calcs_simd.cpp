#include "defines_and_types.hpp"

CalculateIterationsData calcs_simd(int max_iterations, SIMD_REGISTER c_r, SIMD_REGISTER c_i)
{
  // Prepare registers
  SIMD_REGISTER value_r = SIMD_SET_ZERO();
  SIMD_REGISTER value_i = SIMD_SET_ZERO();
  SIMD_REGISTER norm = SIMD_SET_ZERO();
  SIMD_REGISTER iteration_counts = SIMD_SET_ZERO();
  // Run the iterations
  for (int iteration = 0; iteration < max_iterations; ++iteration) {
    // Square ourselves and add c
    SIMD_REGISTER a = value_r;
    SIMD_REGISTER b = value_i;
    value_r = _mm256_sub_pd(_mm256_fmadd_pd(a, a, c_r), _mm256_mul_pd(b, b));
    value_i = _mm256_fmadd_pd(_mm256_mul_pd(_mm256_set1_pd(2.0), a), b, c_i);
    // Compare the norms against the threshold
    norm =  _mm256_fmadd_pd(value_r, value_r, _mm256_mul_pd(value_i, value_i));
    const SIMD_REGISTER norm_lt_threshold = _mm256_cmp_pd(norm, SIMD_SET_ONE(sqrt(5.0)), _CMP_LE_OS);
    // Mask out the data above the threshold and increase the thresholds
    iteration_counts = _mm256_add_pd(iteration_counts, SIMD_AND(SIMD_SET_ONE(1.0), norm_lt_threshold));
  }
  return {norm, iteration_counts};
};
