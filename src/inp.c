#include "inp.h"
#include "GLFW/glfw3.h"
#include "win.h"

static uint32_t key_to_glfw(key_t key);

void inp_create(inp_t* inp, win_t* win) {
  if (!inp)
    return;

  *inp = (inp_t){0};

  if (!win || !win->handle)
    return;

  inp->win = win;

  glfwSetInputMode(win->handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  if (glfwRawMouseMotionSupported())
    glfwSetInputMode(win->handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

  glfwGetCursorPos(win->handle, &inp->mouse_x, &inp->mouse_y);
  inp->initialized = 1;
}

uint8_t inp_key_pressed(inp_t* inp, key_t key) {
  if (!inp || !inp->initialized)
    return 0;

  return glfwGetKey(inp->win->handle, key_to_glfw(key)) == GLFW_PRESS;
}

inp_mouse_delta_t inp_get_mouse_delta(inp_t* inp) {
  inp_mouse_delta_t delta = {0};
  if (!inp || !inp->initialized)
    return delta;

  double mouse_x = 0.0;
  double mouse_y = 0.0;
  glfwGetCursorPos(inp->win->handle, &mouse_x, &mouse_y);

  delta.x = (float)(mouse_x - inp->mouse_x);
  delta.y = (float)(inp->mouse_y - mouse_y);

  inp->mouse_x = mouse_x;
  inp->mouse_y = mouse_y;

  return delta;
}

void inp_destroy(inp_t* inp) {
  if (!inp || !inp->initialized)
    return;

  if (inp->win && inp->win->handle) {
    if (glfwRawMouseMotionSupported())
      glfwSetInputMode(inp->win->handle, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
    glfwSetInputMode(inp->win->handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }

  *inp = (inp_t){0};
}

static uint32_t key_to_glfw(key_t key) {
  switch (key) {
    case KEY_W:
      return GLFW_KEY_W;
    case KEY_A:
      return GLFW_KEY_A;
    case KEY_S:
      return GLFW_KEY_S;
    case KEY_D:
      return GLFW_KEY_D;
    case KEY_ESCAPE:
      return GLFW_KEY_ESCAPE;
    case KEY_E:
      return GLFW_KEY_E;
    case KEY_Q:
      return GLFW_KEY_Q;
    case KEY_SPACE:
      return GLFW_KEY_SPACE;
    case KEY_LSHIFT:
      return GLFW_KEY_LEFT_SHIFT;
    default:
      return GLFW_KEY_UNKNOWN;
  }
}
