#pragma once

#include <stdint.h>
#include "win.h"

typedef enum key_t {
  KEY_W = 0,
  KEY_A = 1,
  KEY_S = 2,
  KEY_D = 3,
  KEY_ESCAPE = 4,
  KEY_E = 5,
  KEY_Q = 6,
  KEY_SPACE = 7,
  KEY_LSHIFT = 8,
} key_t;

typedef struct inp_mouse_delta_t {
  float x;
  float y;
} inp_mouse_delta_t;

typedef struct inp_t {
  win_t* win;
  double mouse_x;
  double mouse_y;
  uint8_t initialized;
} inp_t;

void inp_create(inp_t* inp, win_t* win);
uint8_t inp_key_pressed(inp_t* inp, key_t key);
inp_mouse_delta_t inp_get_mouse_delta(inp_t* inp);
void inp_destroy(inp_t* inp);
