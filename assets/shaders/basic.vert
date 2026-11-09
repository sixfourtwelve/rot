#version 410 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;

uniform mat4 mvp;

out vec2 uv;

void main()
{
  gl_Position = mvp * vec4(pos, 1.0);
  uv = tex;
}
