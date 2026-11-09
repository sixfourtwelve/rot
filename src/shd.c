#include "shd.h"

#include <stdio.h>
#include <stdlib.h>
#include "glad/gl.h"
#include "log.h"

static char* read_file(const char* path) {
  FILE* file = fopen(path, "rb");
  if (!file)
    return NULL;

  if (fseek(file, 0, SEEK_END) != 0) {
    fclose(file);
    return NULL;
  }

  long length = ftell(file);
  if (length < 0 || fseek(file, 0, SEEK_SET) != 0) {
    fclose(file);
    return NULL;
  }

  char* source = malloc((size_t)length + 1);
  if (!source) {
    fclose(file);
    return NULL;
  }

  size_t count = fread(source, 1, (size_t)length, file);
  fclose(file);

  if (count != (size_t)length) {
    free(source);
    return NULL;
  }

  source[count] = '\0';
  return source;
}

static GLuint shd_compile(GladGLContext* gl, GLenum type, const char* source) {
  GLuint id = gl->CreateShader(type);
  if (!id)
    return 0;

  gl->ShaderSource(id, 1, &source, NULL);
  gl->CompileShader(id);

  GLint success = 0;
  gl->GetShaderiv(id, GL_COMPILE_STATUS, &success);

  if (!success) {
    char log[1024] = {0};
    gl->GetShaderInfoLog(id, sizeof(log), NULL, log);
    fprintf(stderr, "Shader compilation failed: %s\n", log);

    gl->DeleteShader(id);
    return 0;
  }

  return id;
}

void shd_create(shd_t* shd, GladGLContext* gl, const char* vertex_path,
                const char* fragment_path) {
  *shd = (shd_t){0};
  shd->gl = gl;

  char* vertex_source = read_file(vertex_path);
  char* fragment_source = read_file(fragment_path);

  GLuint vertex = 0;
  GLuint fragment = 0;
  GLuint program = 0;

  if (!vertex_source || !fragment_source) {
    log_error("Failed to read shader files\n");
    goto cleanup;
  }

  vertex = shd_compile(gl, GL_VERTEX_SHADER, vertex_source);

  fragment = shd_compile(gl, GL_FRAGMENT_SHADER, fragment_source);

  if (!vertex || !fragment)
    goto cleanup;

  program = gl->CreateProgram();
  if (!program)
    goto cleanup;

  gl->AttachShader(program, vertex);
  gl->AttachShader(program, fragment);
  gl->LinkProgram(program);

  GLint success = 0;
  gl->GetProgramiv(program, GL_LINK_STATUS, &success);

  if (!success) {
    char log[1024] = {0};
    gl->GetProgramInfoLog(program, sizeof(log), NULL, log);
    log_error("Shader linking failed: %s", log);
    goto cleanup;
  }

  shd->program = program;
  shd->initialized = 1;
  program = 0;

cleanup:
  if (program)
    gl->DeleteProgram(program);

  if (vertex)
    gl->DeleteShader(vertex);

  if (fragment)
    gl->DeleteShader(fragment);

  free(vertex_source);
  free(fragment_source);
}

void shd_destroy(shd_t* shader) {
  if (shader->program)
    shader->gl->DeleteProgram(shader->program);

  *shader = (shd_t){0};
}

void shd_bind(const shd_t* shader) {
  if (shader->initialized)
    shader->gl->UseProgram(shader->program);
}

void shd_set_vec2(const shd_t* shader, const char* name, const vec2_t value) {
  if (!shader->initialized)
    return;

  GladGLContext* gl = shader->gl;

  GLint location = gl->GetUniformLocation(shader->program, name);

  if (location < 0) {
    log_info("Uniform not found %s", name);
    return;
  }

  gl->Uniform2f(location, value.x, value.y);
}

void shd_set_vec3(const shd_t* shader, const char* name, const vec3_t value) {
  if (!shader->initialized)
    return;

  GladGLContext* gl = shader->gl;

  GLint location = gl->GetUniformLocation(shader->program, name);

  if (location < 0) {
    log_info("Uniform not found %s", name);
    return;
  }

  gl->Uniform3f(location, value.x, value.y, value.z);
}

void shd_set_vec4(const shd_t* shader, const char* name, const vec4_t value) {
  if (!shader->initialized)
    return;

  GladGLContext* gl = shader->gl;

  GLint location = gl->GetUniformLocation(shader->program, name);

  if (location < 0) {
    log_info("Uniform not found %s", name);
    return;
  }

  gl->Uniform4f(location, value.x, value.y, value.z, value.w);
}

void shd_set_mat4(const shd_t* shader, const char* name, const mat44_t value) {
  if (!shader->initialized)
    return;

  GladGLContext* gl = shader->gl;

  GLint location = gl->GetUniformLocation(shader->program, name);

  if (location < 0) {
    log_info("Uniform not found %s", name);
    return;
  }

  gl->UniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat*)&value);
}
