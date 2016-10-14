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


#include "Scene.h"
#include "TexQuad.h"

#include <algorithm>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <cmath>

#include <png/lodepng.h>


using namespace proto;



Scene::Scene ()
 : zoom(1.0),
   window_width(640.0),
   window_height(480.0),
   do_arrange(false),
   do_mipmap(false),
   uniform_modelview(-1),
   uniform_projection(-1),
   uniform_texture(-1)

{
}

Scene::~Scene ()
{
  Unload ();
}

void Scene::Load ()
{
  // Load Geometry
  float z = 0.0;

  // vert 0
  vertex_array[0] = 1.0;   vertex_array[1] = -1.0;   vertex_array[2] = z;  //xyz
  vertex_array[3] = 1.0;   vertex_array[4] = 0.0;                          //uv
  // vert 1
  vertex_array[5] = 1.0;   vertex_array[6] = 1.0;   vertex_array[7] = z;   //xyz
  vertex_array[8] = 1.0;   vertex_array[9] = 1.0;                          //uv
  // vert 2
  vertex_array[10] = -1.0; vertex_array[11] = 1.0;   vertex_array[12] = z; //xyz
  vertex_array[13] = 0.0;  vertex_array[14] = 1.0;                         //uv
  // vert 3
  vertex_array[15] = -1.0; vertex_array[16] = -1.0;   vertex_array[17] = z;//xyz
  vertex_array[18] = 0.0;  vertex_array[19] = 0.0;                         //uv

  // triangle 1
  index_array[0] = 0;   index_array[1] = 1;   index_array[2] = 2;
  // triangle 2
  index_array[3] = 2;   index_array[4] = 3;   index_array[5] = 0;

  glGenVertexArrays (1, &vao);
  glBindVertexArray (vao);

  glGenBuffers (1, &vertex_vbo);
  glBindBuffer (GL_ARRAY_BUFFER, vertex_vbo);
  glBufferData (GL_ARRAY_BUFFER, sizeof(vertex_array), vertex_array, GL_STATIC_DRAW);

  glEnableVertexAttribArray (vPosition);
  glVertexAttribPointer (vPosition, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 0);

  glEnableVertexAttribArray (vTexCoord);
  glVertexAttribPointer (vTexCoord, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));

  glGenBuffers (1, &index_vbo);
  glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, index_vbo);
  glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(index_array), index_array, GL_STATIC_DRAW);


  // Load shaders
  vertex_shader_source = Util::loadShaderSourceFromFile ("simple.vert");
  fragment_shader_source = Util::loadShaderSourceFromFile ("simple.frag");
  program = glCreateProgram ();
  GLuint vert_shader = 0, frag_shader = 0;
  if (!Util::compileShader (vert_shader, GL_VERTEX_SHADER, vertex_shader_source.c_str()))
    std::cout << "Failed to compile vertex shader" << std::endl;
  if (!Util::compileShader (frag_shader, GL_FRAGMENT_SHADER, fragment_shader_source.c_str()))
    std::cout << "Failed to compile fragment shader" << std::endl;

  glAttachShader (program, vert_shader);
  glAttachShader (program, frag_shader);

  // bind vertex attrib locations prior to linking
  glBindAttribLocation (program, vPosition, "a_position");
  glBindAttribLocation (program, vTexCoord, "a_texcoord");
  if (!Util::linkProgram (program))
    { std::cout << "Failed to link program" << std::endl;
      glDeleteShader (vert_shader);
      glDeleteShader (frag_shader);
      glDeleteProgram (program);
    }

  // get uniform locations
  uniform_modelview = glGetUniformLocation (program, "u_modelView");
  uniform_projection = glGetUniformLocation (program, "u_projection");
  uniform_texture = glGetUniformLocation (program, "u_texture");

  if (uniform_modelview == -1 || uniform_projection == -1 || uniform_texture == -1)
    std::cout << "Failed to get a uniform location for one of the shader params" << std::endl;

  if (vert_shader && frag_shader)
   { glDetachShader (program, vert_shader);
     glDetachShader (program, frag_shader);
     glDeleteShader (vert_shader);
     glDeleteShader (frag_shader);
   }
  else
   { std::cout << "whoops" << std::endl; }

  // setup camera
  float fov = 45.0f;  // in degrees, but glm is expecting radians
  float aspect(window_width/window_height);
  float nearz = 0.01f;
  float farz = 10.0f;
  projection = glm::perspective (glm::radians(fov), aspect, nearz, farz);
  glm::mat4 model (1.0f);
  float x = tanf(M_PI_2 - glm::radians(fov/2.0f));  // cotangent
  view = glm::lookAt (
      glm::vec3 (0.0f, 0.0f, x),    // camera position in world space
      glm::vec3 (0.0f, 0.0f, 0.0f), // looking at the origin
      glm::vec3 (0.0f, 1.0f, 0.0f)  // up vector
  );
  glm::mat4 modelView = view * model;


  // use program and set uniforms
  glUseProgram (program);
  glUniformMatrix4fv (uniform_modelview, 1, GL_FALSE, glm::value_ptr(modelView));
  glUniformMatrix4fv (uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
  glUniform1i (uniform_texture, GL_TEXTURE0 - GL_TEXTURE0);

  // make texquads
  const size_t num_images = image_paths.size();
  std::cout << "num images " << num_images << " (" << aspect << ")" << std::endl;
  size_t rows = 0, columns = 0;
  if (aspect > 1.0)
   {
     rows = std::max (1.0, std::floor(double(num_images) / double(aspect)));
     columns = std::ceil(double(num_images) / double(rows));
   }
  else if (aspect < 1.0)
   {
     columns = std::max (1.0, std::floor(double(num_images) * double(aspect)));
     rows = std::ceil(double(num_images) / double(columns));
   }
  else
   {
     const size_t root = static_cast<size_t> (std::ceil (std::sqrt (num_images)));
     rows = num_images / root + (num_images % root ? 1 : 0);
     columns = root;
   }
  std::cout << "Geometry: " << columns << " x " << rows << std::endl;

  glm::vec2 starting_scale(1.0f);  // camera set for vertically centered -1 to 1 square
  if (do_arrange)
   {
     size_t maxdim = std::max (rows, columns);
     size_t mindim = std::min (rows, columns);
     if (aspect > 1.0) {
       starting_scale = glm::vec2( std::min( std::floor(aspect) / float(maxdim),
                                             1.0f / float(mindim)) );
     } else {
       starting_scale = glm::vec2( std::min( std::floor(1.0f/aspect) / float(maxdim),
                                             1.0f / float(mindim)) );
     }
   }
  if (aspect < 1.0)
    starting_scale *= aspect;

  std::cout << "Starting scale " <<  glm::to_string (starting_scale) << std::endl;

  TexQuad *bg = new TexQuad(image_paths[0],
                            aspect,
                            glm::vec3(0.5f),
                            starting_scale,
                            do_mipmap,
                            false);
  bg -> Setup ();
  float q = tanf(M_PI_2 - glm::radians(fov/2.0f));  // cotangent
  glm::mat4 hack = glm::lookAt (
    glm::vec3 (0.0f, 0.0f, q+1),    // camera position in world space
    glm::vec3 (0.0f, 0.0f, 0.0f), // looking at the origin
    glm::vec3 (0.0f, 1.0f, 0.0f)  // up vector
  );
  bg -> SetViewMatrix (uniform_modelview, hack);
  texquads.push_back(bg);
  if (do_arrange)
   { const float pad = 0.1;
     const float size = 2.0f + pad;  // square -1 -> 1
     glm::vec3 pos(0.0f);
     int i = 0;
     for (float r = float(rows-1)*0.5; r > -float(rows)*0.5; r -= 1.0f)
       { std::cout << "row " << r << std::endl;
         pos.y = size * r;
         for (float c = -float(columns-1)*0.5; c < float(columns)*0.5; c += 1.0f)
            { if (i < num_images)
              { std::cout << "col " << c << std::endl;
                pos.x = size * c;
                std::cout << glm::to_string(pos) << std::endl;
                TexQuad *tq = new TexQuad(image_paths[i],
                                          aspect,
                                          pos,
                                          starting_scale,
                                          do_mipmap,
                                          do_arrange);
                tq -> Setup ();
                tq -> SetViewMatrix (uniform_modelview, view);
                texquads.push_back (tq);
                i++;
              }
           }
       }
   }
  else
   { for (int i = 0; i < image_paths.size(); i++)
      { TexQuad *tq = new TexQuad(image_paths[i],
                                  aspect,
                                  glm::vec3(0.0f),
                                  starting_scale,
                                  do_mipmap,
                                  do_arrange);
        tq -> Setup ();
        tq -> SetViewMatrix (uniform_modelview, view);
        texquads.push_back (tq);
      }
   }
}

void Scene::Unload ()
{
  glDeleteProgram (program);

  glBindVertexArray (vao);
  glDeleteBuffers (1, &vertex_vbo);
  glDeleteBuffers (1, &index_vbo);
  glDeleteVertexArrays (1, &vao);
  glBindVertexArray (0);

  for (int i = 0; i < texquads.size(); i++)
    delete texquads[i];
}

void Scene::Setup ()
{ Load ();
  //glClearColor ( 0.0, 0.0, 0.0, 0.0 );  // uncomment for Windows
  glClearColor ( 0.0, 0.1, 0.2, 0.0 );
  glEnable (GL_DEPTH_TEST);
}

void Scene::Update ()
{
}

void Scene::Draw ()
{ glClear (GL_DEPTH_BUFFER_BIT);
  for (int i = 0; i < texquads.size(); i++) {
    texquads[i] -> Update ();
    texquads[i] -> Bind ();
    glDrawElements (GL_TRIANGLE_STRIP, sizeof(index_array)/sizeof(index_array[0]), GL_UNSIGNED_SHORT, 0);
    texquads[i] -> Unbind ();
  }
}

