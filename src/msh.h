#pragma once

#include <stddef.h>
#include <stdint.h>
#include <glad/gl.h>
#include <vecmath.h>

#define MAX_ATTRIBS 16

typedef struct Vertex {
  vec3_t pos;
  vec3_t col;
  vec2_t uv;
} Vertex;

typedef struct buf_t {
  uint32_t vao;
  uint32_t vbo;
  uint32_t ebo;
  size_t count;
} buf_t;

typedef struct msh_attrib_t {
  uint32_t location;
  uint32_t component_count;
  GLenum type;
  uint8_t normalized;
  size_t offset;
} msh_attrib_t;

typedef struct msh_t {
  GladGLContext* gl;
  msh_attrib_t attribs[MAX_ATTRIBS];
  uint8_t initialized;
  buf_t buf;
} msh_t;

typedef struct msh_desc_t {
  GladGLContext* gl;
  const void* vertices;
  const void* indices;
  size_t vertices_count;
  size_t indices_count;
  size_t stride;
  msh_attrib_t attribs[MAX_ATTRIBS];
  uint32_t msh_attrib_count;
} msh_desc_t;

void msh_create(msh_t* msh, msh_desc_t* desc);
void msh_draw(msh_t* msh);
void msh_destroy(msh_t* msh);
