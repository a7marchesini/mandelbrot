//
// Created by Alessandro on 9/19/2023.
//

#include <Eigen/Dense>
#include <unsupported/Eigen/FFT>
#include <raylib.h>


#include <mutex>
#include <thread>

static Eigen::VectorXcf fft_results_shared;
static std::mutex fft_results_shared_mutex;

void Callback(void *bufferData, unsigned int frames)
{
//  printf("Callback started\n");
  fflush(stdout);
  static const int channels = 2;
  Eigen::FFT<float> fft;
  static Eigen::VectorXcf fft_results;
  Eigen::Map<Eigen::VectorXf, Eigen::Unaligned, Eigen::InnerStride<2>> map((float*)bufferData, frames / 2);
  fft.fwd(fft_results, map);

  // Copy to shared data
  {
    std::lock_guard<std::mutex> lock(fft_results_shared_mutex);
//    printf("p1 = %p\n", fft_results_shared.data());
    fft_results_shared = fft_results;
//    printf("p2 = %p\n", fft_results_shared.data());
  }

//  printf("Callback done\n");
//  fflush(stdout);
}

int main() {
  InitWindow(800, 600, "Mandelbrot");

  if (!IsWindowReady()) {
    CloseWindow();
    return 1;
  }

  InitAudioDevice();
  if (!IsAudioDeviceReady())
  {
    CloseWindow();
    return 1;
  }

  SetMasterVolume(.05);

  Music song = LoadMusicStream("C:\\Users\\Alessandro\\Documents\\Documents\\Toby Fox - Megalovania.ogg");

  AttachAudioStreamProcessor(song.stream, Callback);

  PlayMusicStream(song);
  UpdateMusicStream(song);

  SetTargetFPS(60);

  for (int i = 0; i < 100000000 && !WindowShouldClose(); ++i)
  {
    UpdateMusicStream(song);

    BeginDrawing();

    ClearBackground(BLACK);
    DrawFPS(10, 10);

    // Copy from shared data
    static Eigen::VectorXcf fft_results;
    {
      std::lock_guard<std::mutex> lock(fft_results_shared_mutex);
      fft_results = fft_results_shared;
    }

    if (fft_results.size() != 0) {
      float rec_x = 0;
      float rec_y = 0;
      for (int freq = 0; freq < 30; ++freq) {
//        printf("%f\n", fft_results[freq].real());
        float height = norm(fft_results_shared[freq]) * 10.0;
        DrawRectangle(rec_x, 450 - height, 5, height, RED);
        rec_x += 15;
      }
//      fflush(stdout);
    }

    EndDrawing();
    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  UnloadMusicStream(song);
  CloseAudioDevice();
  CloseWindow();

  return 0;
}

