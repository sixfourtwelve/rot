#include "cam.h"
#include "vecmath.h"

void cam_create(cam_t* cam, float fov) {
  if (!cam)
    return;

  *cam = (cam_t){0};

  cam->eye = vec3(0.0f, 1.5f, 4.0f);
  cam->forward = vec3_normalize(vec3_sub(vec3(0.0f, 0.0f, 0.0f), cam->eye));
  cam->up = vec3(0.0f, 1.0f, 0.0f);
  cam->yaw = vecmath_degrees(vecmath_atan2(cam->forward.z, cam->forward.x));
  cam->pitch = vecmath_degrees(vecmath_asin(cam->forward.y));
  cam->fov = fov;
  cam->speed = 2.1f;
  cam->mouse_sensitivity = 0.08f;

  cam->initialized = 1;
}

void cam_move(cam_t* cam, cam_movement_t movement, float delta_time) {
  if (!cam || !cam->initialized || delta_time <= 0.0f)
    return;

  vec3_t forward = vec3_normalize(cam->forward);
  vec3_t right = vec3_normalize(vec3_cross(forward, cam->up));
  vec3_t direction;

  switch (movement) {
    case MOVE_FORWARD:
      direction = forward;
      break;

    case MOVE_BACKWARD:
      direction = vec3_mulf(forward, -1.0f);
      break;

    case MOVE_LEFT:
      direction = vec3_mulf(right, -1.0f);
      break;

    case MOVE_RIGHT:
      direction = right;
      break;

    default:
      return;
  }

  float distance = cam->speed * delta_time;
  cam->eye = vec3_add(cam->eye, vec3_mulf(direction, distance));
}

void cam_look(cam_t* cam, float delta_x, float delta_y) {
  if (!cam || !cam->initialized)
    return;

  cam->yaw += delta_x * cam->mouse_sensitivity;
  cam->pitch += delta_y * cam->mouse_sensitivity;
  cam->pitch = vecmath_clamp(cam->pitch, -89.0f, 89.0f);

  float yaw = vecmath_radians(cam->yaw);
  float pitch = vecmath_radians(cam->pitch);
  float pitch_cos = vecmath_cos(pitch);

  vec3_t forward = vec3(vecmath_cos(yaw) * pitch_cos, vecmath_sin(pitch),
                        vecmath_sin(yaw) * pitch_cos);
  cam->forward = vec3_normalize(forward);
}

mat44_t cam_get_view_projection(cam_t* cam, int width, int height) {
  if (!cam || !cam->initialized || width <= 0 || height <= 0)
    return mat44_identity();

  vec3_t target = vec3_add(cam->eye, cam->forward);

  cam->projection = mat44_perspective_fov_rh(
      vecmath_radians(cam->fov), (float)width / (float)height, 0.01f, 100.0f);

  cam->view = mat44_look_at_rh(cam->eye, target, cam->up);

  cam->view_projection = mat44_mul_mat44(cam->view, cam->projection);

  return cam->view_projection;
}

void cam_destroy(cam_t* cam) {
  if (!cam || !cam->initialized)
    return;

  *cam = (cam_t){0};
}
