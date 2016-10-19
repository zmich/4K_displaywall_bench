#version 440 core

in vec3 a_position;
in vec2 a_texcoord;

uniform mat4 u_modelView;
uniform mat4 u_projection;

out vec2 v_texcoord;

void main () {
  gl_Position = u_projection * u_modelView * vec4(a_position.xyz, 1.0);
}
