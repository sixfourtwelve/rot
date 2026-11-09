#pragma once

#include <GLFW/glfw3.h>
#include <stdint.h>

typedef struct win_params_t {
  uint32_t width, height;
  const char* title;
  uint8_t vsync;
  uint8_t resizable;
  uint8_t maximized;
} win_params_t;

typedef struct win_t {
  GLFWwindow* handle;
  win_params_t params;
  uint8_t initialized;
} win_t;

void win_create(win_t* win, win_params_t params);
void win_destroy(win_t* win);
void win_set_should_close(win_t* win, uint8_t value);
void win_set_title(win_t* win, const char* title);
uint8_t win_should_close(win_t* win);
void win_poll_events(void);
void win_swap(win_t* win);
uint8_t win_key_pressed(win_t* win, int key);
