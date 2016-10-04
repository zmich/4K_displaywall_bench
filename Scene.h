
#ifndef __Scene__proto__
#define __Scene__proto__

#include <iostream>

// glad extension loader: https://github.com/Dav1dde/glad
#include <glad/glad.h>

// GLFW: glfw.org
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

// OpenGL Math Library: glm.g-truc.net
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "util.h"

namespace proto {

class Scene {

public:
  Scene ();
  ~Scene ();

  void Setup ();
  void Update ();
  void Draw ();

  void ResetZoom ()
    { zoom = 1.0; }

  void IncrementZoom ()
    { zoom *= 1.1; }

  void DecrementZoom ()
    { zoom *= 0.9; }

  void SetWindow (float w, float h)
    { window_width = w;  window_height = h; }

public:
  enum vertexAttrib {
    vPosition,
    vTexCoord
  };

protected:
  void Load ();
  void Unload ();

protected:
  float zoom;

  float window_width;
  float window_height;

  GLuint vao;
  GLuint index_vbo;
  GLuint vertex_vbo;

  GLfloat vertex_array[20];
  GLushort index_array[6];

  std::string vertex_shader_source;
  std::string fragment_shader_source;
  GLuint program;

};

}

#endif  // defined(__Scene__proto__)
