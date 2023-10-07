//
// Created by Alessandro on 10/6/2023.
//

#ifndef MANDELBROT_FN_DEFS_HPP
#define MANDELBROT_FN_DEFS_HPP

#include "defines_and_types.hpp"

#include <semaphore>

CalculateIterationsData calcs_scalar(int iterations, SIMD_REGISTER c_r, SIMD_REGISTER c_i);
CalculateIterationsData calcs_simd(int iterations, SIMD_REGISTER c_r, SIMD_REGISTER c_i);

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
  const bool *p_exit_flag);

void user_input(
  Eigen::Vector2d *p_x_axis,
  Eigen::Vector2d *p_y_axis,
  double *p_pan_speed,
  double *p_zoom_speed,
  EIGEN_VECTOR *p_x_pixels_pos,
  EIGEN_VECTOR *p_y_pixels_pos,
  int *p_iterations);

void reinit(
  EIGEN_VECTOR *p_x_pixels_pos,
  EIGEN_VECTOR *p_y_pixels_pos,
  Eigen::VectorXi *p_thread_y_pixel_ranges,
  Colors *p_colors,
  Image *p_image,
  Texture2D *p_texture);

#endif //MANDELBROT_FN_DEFS_HPP
