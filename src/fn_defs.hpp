//
// Created by Alessandro on 10/6/2023.
//

#ifndef MANDELBROT_FN_DEFS_HPP
#define MANDELBROT_FN_DEFS_HPP

#include "defines_and_types.hpp"

#include <mutex>
#include <semaphore>

void calcs_scalar(int max_iterations, int y, EIGEN_VECTOR *p_x_pixels_pos, EIGEN_VECTOR *p_y_pixels_pos, EIGEN_MATRIX *p_norms, IterationCounts *p_iteration_counts);
CalculateIterationsData calcs_simd(int iterations, SIMD_REGISTER c_r, SIMD_REGISTER c_i);

void thread_fn(
  int *p_iterations,
  int *p_calc_choice,
  EIGEN_VECTOR *p_x_pixels_pos,
  EIGEN_VECTOR *p_y_pixels_pos,
  EIGEN_MATRIX *p_norms,
  IterationCounts *p_iteration_counts,
  int *p_y_next,
  std::mutex *p_y_next_mut,
  std::counting_semaphore<INT_MAX> *p_semaphore_main_to_thread,
  std::counting_semaphore<INT_MAX> *p_semaphore_thread_to_main,
  const bool *p_exit_flag);

void user_input(
  Eigen::Vector2d *p_x_axis,
  Eigen::Vector2d *p_y_axis,
  double *p_pan_speed,
  double *p_zoom_speed,
  EIGEN_VECTOR *p_x_pixels_pos,
  EIGEN_VECTOR *p_y_pixels_pos,
  int *p_iterations,
  int *p_calc_choice);

void reinit(
  Colors *p_colors,
  Image *p_image,
  Texture2D *p_texture,
  EIGEN_VECTOR *p_x_pixels_pos,
  EIGEN_VECTOR *p_y_pixels_pos,
  EIGEN_MATRIX *p_norms,
  IterationCounts *p_iteration_counts);

#endif //MANDELBROT_FN_DEFS_HPP
