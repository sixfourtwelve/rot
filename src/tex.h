#pragma once

#include <stdint.h>
#include "glad/gl.h"

typedef struct tex_t {
  uint32_t tex;
  GladGLContext* gl;
  uint8_t initialized;
} tex_t;

void tex_create(tex_t* tex, GladGLContext* gl, const char* tex_path);
void tex_bind(tex_t* tex);
void tex_destroy(tex_t* tex);
