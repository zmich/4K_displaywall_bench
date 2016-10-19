#version 440 core

in vec2 v_texcoord;
uniform sampler2D u_texture;

out vec4 frag_color;


void main ()
{
  frag_color.rgb = texture2D(u_texture, v_texcoord).rgba;
}
