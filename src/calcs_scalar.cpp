#include "defines_and_types.hpp"

// Create lambda to calculate iterations
CalculateIterationsData calcs_scalar(int iterations, SIMD_REGISTER c_r, SIMD_REGISTER c_i)
{
  SIMD_REGISTER norm = SIMD_SET_ZERO();
  SIMD_REGISTER iteration_counts = SIMD_SET_ZERO();
  // Prepare registers
  for (int i = 0; i < VALUES_PER_REGISTER; ++i) {
    VALUE_TYPE value_r = 0;
    VALUE_TYPE value_i = 0;
    // Run the iterations
    for (int iteration = 0; iteration < iterations; ++iteration) {
      // Square ourselves and add c
      VALUE_TYPE a = value_r;
      VALUE_TYPE b = value_i;
      // Bind a and b to prevent spiraling out to infinity
      a = std::max(a, VALUE_TYPE(-2.0));
      a = std::min(a, VALUE_TYPE(2.0));
      b = std::max(b, VALUE_TYPE(-2.0));
      b = std::min(b, VALUE_TYPE(2.0));
      value_r = a * a - b * b + c_r[i];
      value_i = VALUE_TYPE(2.0) * a * b + c_i[i];
      // Compare the norms against the threshold
      norm[i] = value_r * value_r + value_i * value_i;
      const VALUE_TYPE norm_lt_threshold = norm[i] < VALUE_TYPE(4.0);
      // Mask out the data above the threshold and increase the thresholds
      iteration_counts[i] += VALUE_TYPE(norm_lt_threshold);
    }
  }
  return {norm, iteration_counts};
};
