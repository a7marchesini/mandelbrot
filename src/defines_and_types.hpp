//
// Created by Alessandro on 10/6/2023.
//

#ifndef MANDELBROT_DEFINES_AND_TYPES_HPP
#define MANDELBROT_DEFINES_AND_TYPES_HPP

#include <Eigen/Dense>
#include <raylib.h>
#include <immintrin.h>

#include <thread>

#define USE_FLOAT 0

#if USE_FLOAT
#define VALUE_TYPE float
#define VALUES_PER_REGISTER 8
#define SIMD_INDEX(reg, ix) reinterpret_cast<float *>(&reg)[ix]
#define SIMD_REGISTER __m256
#define SIMD_SET_ZERO _mm256_setzero_ps
#define SIMD_SET_ONE _mm256_set1_ps
#define SIMD_LOAD _mm256_load_ps
#define SIMD_MIN _mm256_min_ps
#define SIMD_MAX _mm256_max_ps
#define SIMD_AND _mm256_and_ps
#define EIGEN_VECTOR Eigen::VectorXf
#define EIGEN_MATRIX Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>
#else
#define VALUE_TYPE double
#define VALUES_PER_REGISTER 4
#define SIMD_INDEX(reg, ix) reinterpret_cast<double *>(&reg)[ix]
#define SIMD_REGISTER __m256d
#define SIMD_SET_ZERO _mm256_setzero_pd
#define SIMD_SET_ONE _mm256_set1_pd
#define SIMD_LOAD _mm256_load_pd
#define SIMD_MIN _mm256_min_pd
#define SIMD_MAX _mm256_max_pd
#define SIMD_AND _mm256_and_pd
#define EIGEN_VECTOR Eigen::VectorXd
#define EIGEN_MATRIX Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>
#endif

#define NUM_THREADS std::thread::hardware_concurrency() // Give audio and graphics a chance to breathe
//#define NUM_THREADS 1

struct CalculateIterationsData
{
  SIMD_REGISTER norm_ = SIMD_SET_ZERO();
  SIMD_REGISTER iteration_counts_ = SIMD_SET_ZERO();
};

using Colors = Eigen::Matrix<Color, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

#define MAX_ITERATION_COLORS 256
using IterationColors = Color[MAX_ITERATION_COLORS];

using IterationCounts = Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

#endif //MANDELBROT_DEFINES_AND_TYPES_HPP
