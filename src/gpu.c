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
      // Front: +Z
      {.pos = {-0.5f, -0.5f, 0.5f}, .col = {1, 1, 1}, .uv = {0, 0}},
      {.pos = {0.5f, -0.5f, 0.5f}, .col = {1, 1, 1}, .uv = {1, 0}},
      {.pos = {0.5f, 0.5f, 0.5f}, .col = {1, 1, 1}, .uv = {1, 1}},
      {.pos = {-0.5f, 0.5f, 0.5f}, .col = {1, 1, 1}, .uv = {0, 1}},

      // Back: -Z
      {.pos = {-0.5f, -0.5f, -0.5f}, .col = {1, 1, 1}, .uv = {0, 0}},
      {.pos = {-0.5f, 0.5f, -0.5f}, .col = {1, 1, 1}, .uv = {0, 1}},
      {.pos = {0.5f, 0.5f, -0.5f}, .col = {1, 1, 1}, .uv = {1, 1}},
      {.pos = {0.5f, -0.5f, -0.5f}, .col = {1, 1, 1}, .uv = {1, 0}},

      // Right: +X
      {.pos = {0.5f, -0.5f, 0.5f}, .col = {1, 1, 1}, .uv = {0, 0}},
      {.pos = {0.5f, -0.5f, -0.5f}, .col = {1, 1, 1}, .uv = {1, 0}},
      {.pos = {0.5f, 0.5f, -0.5f}, .col = {1, 1, 1}, .uv = {1, 1}},
      {.pos = {0.5f, 0.5f, 0.5f}, .col = {1, 1, 1}, .uv = {0, 1}},

      // Left: -X
      {.pos = {-0.5f, -0.5f, -0.5f}, .col = {1, 1, 1}, .uv = {0, 0}},
      {.pos = {-0.5f, -0.5f, 0.5f}, .col = {1, 1, 1}, .uv = {1, 0}},
      {.pos = {-0.5f, 0.5f, 0.5f}, .col = {1, 1, 1}, .uv = {1, 1}},
      {.pos = {-0.5f, 0.5f, -0.5f}, .col = {1, 1, 1}, .uv = {0, 1}},

      // Top: +Y
      {.pos = {-0.5f, 0.5f, 0.5f}, .col = {1, 1, 1}, .uv = {0, 0}},
      {.pos = {0.5f, 0.5f, 0.5f}, .col = {1, 1, 1}, .uv = {1, 0}},
      {.pos = {0.5f, 0.5f, -0.5f}, .col = {1, 1, 1}, .uv = {1, 1}},
      {.pos = {-0.5f, 0.5f, -0.5f}, .col = {1, 1, 1}, .uv = {0, 1}},

      // Bottom: -Y
      {.pos = {-0.5f, -0.5f, -0.5f}, .col = {1, 1, 1}, .uv = {0, 0}},
      {.pos = {0.5f, -0.5f, -0.5f}, .col = {1, 1, 1}, .uv = {1, 0}},
      {.pos = {0.5f, -0.5f, 0.5f}, .col = {1, 1, 1}, .uv = {1, 1}},
      {.pos = {-0.5f, -0.5f, 0.5f}, .col = {1, 1, 1}, .uv = {0, 1}},
  };

  const uint32_t indices[] = {
      0,  1,  2,  2,  3,  0,   // Front
      4,  5,  6,  6,  7,  4,   // Back
      8,  9,  10, 10, 11, 8,   // Right
      12, 13, 14, 14, 15, 12,  // Left
      16, 17, 18, 18, 19, 16,  // Top
      20, 21, 22, 22, 23, 20,  // Bottom
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
  msh_desc_t desc = {
      .gl = gl,
      .vertices = vertices,
      .indices = indices,
      .vertices_count = ARRAY_SIZE(vertices),
      .indices_count = ARRAY_SIZE(indices),
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

  gl->Enable(GL_DEPTH_TEST);

  gl->Enable(GL_CULL_FACE);
  gl->CullFace(GL_BACK);
  gl->FrontFace(GL_CCW);
}

void gpu_begin(gpu_t* gpu) {
  if (!gpu->gpu_initialized)
    return;

  GladGLContext* gl = &gpu->gl;

  glfwGetFramebufferSize(gpu->win->handle, &gpu->width, &gpu->height);

  gl->Viewport(0, 0, gpu->width, gpu->height);
  gl->ClearColor(0.10f, 0.10f, 0.10f, 1.0f);
  gl->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gpu_end(gpu_t* gpu) {
  if (!gpu->gpu_initialized)
    return;

  shd_bind(&gpu->shd);

  float time = (float)glfwGetTime();

  mat44_t model =
      mat44_mul_mat44(mat44_rotation_x(time * 0.5f), mat44_rotation_y(time));

  mat44_t view_projection =
      cam_get_view_projection(gpu->cam, gpu->width, gpu->height);

  mat44_t mvp = mat44_mul_mat44(model, view_projection);

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
