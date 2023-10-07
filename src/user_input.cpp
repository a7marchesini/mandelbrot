
#include "defines_and_types.hpp"

void user_input(
  Eigen::Vector2d *p_x_axis,
  Eigen::Vector2d *p_y_axis,
  double *p_pan_speed,
  double *p_zoom_speed,
  EIGEN_VECTOR *p_x_pixels_pos,
  EIGEN_VECTOR *p_y_pixels_pos,
  int *p_iterations)
{
  // Increase or decrease pan speed
  if (IsKeyPressed(KEY_T))
  {
    *p_pan_speed -= 0.01;
  }
  if (IsKeyPressed(KEY_Y))
  {
    *p_pan_speed += 0.01;
  }
  // Increase or decrease zoom speed
  if (IsKeyPressed(KEY_U))
  {
    *p_zoom_speed -= 0.01;
  }
  if (IsKeyPressed(KEY_I))
  {
    *p_zoom_speed += 0.01;
  }

  // Screen ranges
  double x_range = (*p_x_axis)[1] - (*p_x_axis)[0];
  double y_range = (*p_y_axis)[1] - (*p_y_axis)[0];

  // Pan across screen
  if (IsKeyDown(KEY_UP))
  {
    p_y_axis->array() -= *p_pan_speed * y_range;
  }
  if (IsKeyDown(KEY_DOWN))
  {
    p_y_axis->array() += *p_pan_speed * y_range;
  }
  if (IsKeyDown(KEY_LEFT))
  {
    p_x_axis->array() -= *p_pan_speed * x_range;
  }
  if (IsKeyDown(KEY_RIGHT))
  {
    p_x_axis->array() += *p_pan_speed * x_range;
  }

  // Recalculate ranges
  x_range = (*p_x_axis)[1] - (*p_x_axis)[0];
  y_range = (*p_y_axis)[1] - (*p_y_axis)[0];

  // Zoom into middle of screen
  if (IsKeyDown(KEY_W))
  {
    double offset;
    offset = *p_zoom_speed * x_range;
    (*p_x_axis)[0] += offset;
    (*p_x_axis)[1] -= offset;
    offset = *p_zoom_speed * y_range;
    (*p_y_axis)[0] += offset;
    (*p_y_axis)[1] -= offset;
  }
  // Zoom out of middle of screen
  if (IsKeyDown(KEY_S))
  {
    double offset;
    offset = *p_zoom_speed * x_range;
    (*p_x_axis)[0] -= offset;
    (*p_x_axis)[1] += offset;
    offset = *p_zoom_speed * y_range;
    (*p_y_axis)[0] -= offset;
    (*p_y_axis)[1] += offset;
  }

  // Targeted at middle of pixel
  p_x_pixels_pos->setLinSpaced((*p_x_axis)[0],  (*p_x_axis)[1]);
  p_x_pixels_pos->array() +=  ((*p_x_axis)[1] - (*p_x_axis)[0]) / GetScreenWidth();
  p_y_pixels_pos->setLinSpaced((*p_y_axis)[0],  (*p_y_axis)[1]);
  p_y_pixels_pos->array() +=  ((*p_y_axis)[1] - (*p_y_axis)[0]) / GetScreenHeight();

  // Increase or decrease iterations
  if (IsKeyPressed(KEY_E))
  {
    *p_iterations -= 16;
  }
  if (IsKeyPressed(KEY_R))
  {
    *p_iterations += 16;
  }

}