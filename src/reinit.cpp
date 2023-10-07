
#include "defines_and_types.hpp"

static bool startup_executed = false;

void reinit(
  EIGEN_VECTOR *p_x_pixels_pos,
  EIGEN_VECTOR *p_y_pixels_pos,
  Eigen::VectorXi *p_thread_y_pixel_ranges,
  Colors *p_colors,
  Image *p_image,
  Texture2D *p_texture)
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
  p_x_pixels_pos->resize(width);
  p_y_pixels_pos->resize(height);
  p_colors->resize(height, width);

  // Define image to be drawn on screen
  *p_image = Image{p_colors->data(), width, height, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
  *p_texture = LoadTextureFromImage(*p_image);

  // Figure out the Y pixel ranges for each thread
  p_thread_y_pixel_ranges->setLinSpaced(NUM_THREADS + 1, 0, height);
  (*p_thread_y_pixel_ranges)[p_thread_y_pixel_ranges->size() - 1] = height; // setLinSpaced will lower the end of the range to satisfy internal conditions
}