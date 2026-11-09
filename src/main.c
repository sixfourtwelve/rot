#include <stdio.h>
#include "GLFW/glfw3.h"
#include "cam.h"
#include "gpu.h"
#include "inp.h"
#include "win.h"

int main(void) {
  win_params_t params = {
      .width = 1280,
      .height = 720,
      .title = "Rot (mode) - FPS: 0",
      .vsync = 1,
      .resizable = 1,
      .maximized = 1,
  };

  win_t win;
  win_create(&win, params);

  cam_t cam;
  cam_create(&cam, 45.f);

  gpu_t gpu;
  gpu_create(&gpu, &win, &cam);

  inp_t inp;
  inp_create(&inp, &win);

  double deltaTime = 0.0f;
  double lastFrame = glfwGetTime();
  double fpsTimer = lastFrame;
  double currentFrame = glfwGetTime();
  int frameCount = 0;

  while (!win_should_close(&win)) {
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    win_poll_events();
    if (inp_key_pressed(&inp, KEY_ESCAPE))
      win_set_should_close(&win, 1);

    inp_mouse_delta_t mouse_delta = inp_get_mouse_delta(&inp);
    cam_look(&cam, mouse_delta.x, mouse_delta.y);

    float dt = (float)deltaTime;

    if (inp_key_pressed(&inp, KEY_W))
      cam_move(&cam, MOVE_FORWARD, dt);
    if (inp_key_pressed(&inp, KEY_A))
      cam_move(&cam, MOVE_LEFT, dt);
    if (inp_key_pressed(&inp, KEY_S))
      cam_move(&cam, MOVE_BACKWARD, dt);
    if (inp_key_pressed(&inp, KEY_D))
      cam_move(&cam, MOVE_RIGHT, dt);

    frameCount++;

    double elapsed = currentFrame - fpsTimer;

    if (elapsed >= 1.0) {
      double fps = frameCount / elapsed;
      char buff[1024];
#if ROT_DEBUG
      snprintf(buff, sizeof(buff), "Rot (development) - FPS: %.0f", fps);
#else
      snprintf(buff, sizeof(buff), "Rot (release) - FPS: %.0f", fps);
#endif
      win_set_title(&win, buff);
      frameCount = 0;
      fpsTimer = currentFrame;
    }

    gpu_begin(&gpu);
    gpu_end(&gpu);
    gpu_submit(&gpu);

    win_swap(&win);
  }

  inp_destroy(&inp);
  cam_destroy(&cam);
  gpu_destroy(&gpu);
  win_destroy(&win);

  return 0;
}
