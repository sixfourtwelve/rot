#pragma once

#include <stdint.h>
#include "vecmath.h"

typedef enum cam_movement_t {
  MOVE_FORWARD = 0,
  MOVE_BACKWARD = 1,
  MOVE_LEFT = 2,
  MOVE_RIGHT = 3,
} cam_movement_t;

typedef struct cam_t {
  float yaw;
  float pitch;
  float fov;
  float speed;
  float mouse_sensitivity;

  vec3_t forward;
  vec3_t up;
  vec3_t eye;

  mat44_t view;
  mat44_t model;
  mat44_t projection;
  mat44_t view_projection;

  uint8_t initialized;
} cam_t;

void cam_create(cam_t* cam, float fov);
void cam_move(cam_t* cam, cam_movement_t movement, float delta_time);
void cam_look(cam_t* cam, float delta_x, float delta_y);
mat44_t cam_get_mvp(cam_t* cam, int width, int height);
void cam_destroy(cam_t* cam);
