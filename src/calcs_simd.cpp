#include "defines_and_types.hpp"

CalculateIterationsData calcs_simd(int iterations, SIMD_REGISTER c_r, SIMD_REGISTER c_i)
{
  // Prepare registers
  SIMD_REGISTER value_r = SIMD_SET_ZERO();
  SIMD_REGISTER value_i = SIMD_SET_ZERO();
  SIMD_REGISTER norm = SIMD_SET_ZERO();
  SIMD_REGISTER iteration_counts = SIMD_SET_ZERO();
  // Run the iterations
  for (int iteration = 0; iteration < iterations; ++iteration) {
    // Square ourselves and add c
    SIMD_REGISTER a = value_r;
    SIMD_REGISTER b = value_i;
    value_r = a * a - b * b + c_r;
    value_i = 2.0 * a * b + c_i;
    // Compare the norms against the threshold
    norm = value_r * value_r + value_i * value_i;
    const SIMD_REGISTER norm_lt_threshold = norm < SIMD_SET_ONE(4.0);
    // Mask out the data above the threshold and increase the thresholds
    iteration_counts += SIMD_AND(SIMD_SET_ONE(1.0), norm_lt_threshold);
  }
  return {norm, iteration_counts};
};
