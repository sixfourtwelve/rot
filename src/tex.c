#include "tex.h"
#include "glad/gl.h"
#include "log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void tex_create(tex_t* tex, GladGLContext* gl, const char* tex_path) {
  tex->gl = gl;

  stbi_set_flip_vertically_on_load(1);
  int width, height, n_channels;
  unsigned char* data = stbi_load(tex_path, &width, &height, &n_channels, 0);
  if (data == NULL) {
    log_error("Tex Path not found: %s", tex_path);
    return;
  }

  gl->GenTextures(1, &tex->tex);
  gl->BindTexture(GL_TEXTURE_2D, tex->tex);
  gl->TexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
  gl->GenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  tex->initialized = 1;
}

void tex_bind(tex_t* tex) {
  GladGLContext* gl = tex->gl;
  gl->BindTexture(GL_TEXTURE_2D, tex->tex);
}

void tex_destroy(tex_t* tex) {
  if (!tex->initialized) {
    return;
  }

  GladGLContext* gl = tex->gl;

  if (tex->tex != 0) {
    gl->DeleteTextures(1, &tex->tex);
  }

  *tex = (tex_t){0};
}
