#pragma once

#include <stdint.h>
#include <glad/gl.h>
#include <vecmath.h>

typedef struct shd_t {
  uint32_t program;
  uint32_t v_shader;
  uint32_t f_shader;
  GladGLContext* gl;
  uint8_t initialized;
} shd_t;

void shd_create(shd_t* shader, GladGLContext* gl, const char* vertex_path,
                const char* fragment_path);

void shd_destroy(shd_t* shader);
void shd_bind(const shd_t* shader);

void shd_set_vec2(const shd_t* shader, const char* name, const vec2_t value);
void shd_set_vec3(const shd_t* shader, const char* name, const vec3_t value);
void shd_set_vec4(const shd_t* shader, const char* name, const vec4_t value);
void shd_set_mat4(const shd_t* shader, const char* name, const mat44_t value);
