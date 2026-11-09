#include "gpu.h"

#include "GLFW/glfw3.h"
#include "cam.h"
#include "glad/gl.h"
#include "log.h"
#include "macros.h"
#include "msh.h"
#include "shd.h"
#include "tex.h"

void gpu_create(gpu_t* gpu, win_t* win, cam_t* cam) {
  *gpu = (gpu_t){0};

  if (!gladLoadGLContext(&gpu->gl, (GLADloadfunc)glfwGetProcAddress)) {
    log_error("Failed to load glad");
    return;
  }

  const Vertex vertices[] = {
      (Vertex){
          .pos = {-0.5f, -0.5f, 0.0f},
          .col = {1.0f, 0.5f, 0.2f},
          .uv = {0.0f, 0.0f},
      },
      (Vertex){
          .pos = {0.5f, -0.5f, 0.0f},
          .col = {0.0f, 0.5f, 0.2f},
          .uv = {1.0f, 0.0f},
      },
      (Vertex){
          .pos = {0.0f, 0.5f, 0.0f},
          .col = {0.3f, 0.5f, 0.2f},
          .uv = {0.5f, 1.0f},
      },
  };

  msh_attrib_t attribs[2] = {
      (msh_attrib_t){
          .location = 0,
          .component_count = 3,
          .type = GL_FLOAT,
          .normalized = GL_FALSE,
          .offset = offsetof(Vertex, pos),
      },
      (msh_attrib_t){
          .location = 1,
          .component_count = 2,
          .type = GL_FLOAT,
          .normalized = GL_FALSE,
          .offset = offsetof(Vertex, uv),
      },
  };

  GladGLContext* gl = &gpu->gl;
  msh_desc_t desc = (msh_desc_t){
      .gl = gl,
      .vertices = vertices,
      .vertices_count = ARRAY_SIZE(vertices),
      .stride = sizeof(Vertex),
      .attribs = {attribs[0], attribs[1]},
      .msh_attrib_count = ARRAY_SIZE(attribs),
  };

  msh_create(&gpu->msh, &desc);

  tex_create(&gpu->tex, gl, "assets/textures/orange/texture_01.png");

  gpu->win = win;
  shd_create(&gpu->shd, &gpu->gl, "assets/shaders/basic.vert",
             "assets/shaders/basic.frag");

  gpu->cam = cam;

  gpu->gpu_initialized = 1;
}

void gpu_begin(gpu_t* gpu) {
  if (!gpu->gpu_initialized)
    return;

  GladGLContext* gl = &gpu->gl;

  glfwGetFramebufferSize(gpu->win->handle, &gpu->width, &gpu->height);

  gl->Viewport(0, 0, gpu->width, gpu->height);
  gl->Clear(GL_COLOR_BUFFER_BIT);
  gl->ClearColor(0.10f, 0.10f, 0.10f, 1.0f);
}

void gpu_end(gpu_t* gpu) {
  if (!gpu->gpu_initialized)
    return;

  shd_bind(&gpu->shd);

  mat44_t mvp = cam_get_mvp(gpu->cam, gpu->width, gpu->height);
  shd_set_mat4(&gpu->shd, "mvp", mvp);
}

void gpu_submit(gpu_t* gpu) {
  if (!gpu->gpu_initialized)
    return;

  if (gpu->tex.initialized)
    tex_bind(&gpu->tex);

  if (gpu->msh.initialized)
    msh_draw(&gpu->msh);
}

void gpu_destroy(gpu_t* gpu) {
  if (!gpu->gpu_initialized)
    return;

  if (gpu->tex.initialized)
    tex_destroy(&gpu->tex);

  if (gpu->shd.initialized)
    shd_destroy(&gpu->shd);

  if (gpu->msh.initialized)
    msh_destroy(&gpu->msh);

  *gpu = (gpu_t){0};
}
