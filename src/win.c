#include "win.h"
#include "GLFW/glfw3.h"
#include "log.h"

void win_destroy(win_t* win);
void win_create(win_t* win, win_params_t params) {
  if (!glfwInit()) {
    log_error("failed to init glfw");
    return;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, params.resizable);

  win->handle = glfwCreateWindow((int)params.width, (int)params.height,
                                 params.title, NULL, NULL);
  if (win->handle == NULL) {
    log_error("failed to create glfw window");
    win_destroy(win);
    return;
  }

  win->initialized = 1;

  glfwMakeContextCurrent(win->handle);
  glfwSwapInterval(params.vsync);
  if (params.maximized)
    glfwMaximizeWindow(win->handle);
}

void win_destroy(win_t* win) {
  if (!win)
    return;

  if (win->handle != NULL)
    glfwDestroyWindow(win->handle);

  if (win->initialized)
    glfwTerminate();
}

void win_set_title(win_t* win, const char* title) {
  glfwSetWindowTitle(win->handle, title);
}

void win_set_should_close(win_t* win, uint8_t value) {
  glfwSetWindowShouldClose(win->handle, value);
}

uint8_t win_should_close(win_t* win) {
  return glfwWindowShouldClose(win->handle);
}

void win_poll_events(void) {
  glfwPollEvents();
}

void win_swap(win_t* win) {
  if (win && win->handle) {
    glfwSwapBuffers(win->handle);
  }
}

uint8_t win_key_pressed(win_t* win, int key) {
  return glfwGetKey(win->handle, key) == GLFW_PRESS;
}
