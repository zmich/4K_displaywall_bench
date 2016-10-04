/* 4K Displaywall Bench
 * Copyright (c) 2016, Oblong Industries, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
