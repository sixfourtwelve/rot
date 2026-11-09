#pragma once

#include <glad/gl.h>
#include <stdint.h>

#include "cam.h"
#include "msh.h"
#include "shd.h"
#include "tex.h"
#include "win.h"

typedef struct gpu_t {
  GladGLContext gl;
  win_t* win;
  shd_t shd;
  uint8_t gpu_initialized;
  buf_t buf;
  msh_t msh;
  tex_t tex;
  cam_t* cam;

  int width;
  int height;
} gpu_t;

void gpu_create(gpu_t* gpu, win_t* win, cam_t* cam);
void gpu_begin(gpu_t* gpu);
void gpu_end(gpu_t* gpu);
void gpu_submit(gpu_t* gpu);
void gpu_destroy(gpu_t* gpu);
