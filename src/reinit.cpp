
#include "defines_and_types.hpp"

static bool startup_executed = false;

void reinit(
  Colors *p_colors,
  Image *p_image,
  Texture2D *p_texture,
  EIGEN_VECTOR *p_x_pixels_pos,
  EIGEN_VECTOR *p_y_pixels_pos,
  EIGEN_MATRIX *p_norms,
  IterationCounts *p_iteration_counts)
{
  // Unload stuff from last reinit
  if (startup_executed) {
    UnloadTexture(*p_texture);
  }
  startup_executed = true;

  // Get new sizes
  int width = GetScreenWidth();
  int height = GetScreenHeight();

  // Resize data containers
  p_colors->resize(height, width);
  p_x_pixels_pos->resize(width);
  p_y_pixels_pos->resize(height);
  p_norms->resize(height, width);
  p_iteration_counts->resize(height, width);

  // Define image to be drawn on screen
  *p_image = Image{p_colors->data(), width, height, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
  *p_texture = LoadTextureFromImage(*p_image);
}