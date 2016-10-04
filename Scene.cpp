


#include "Scene.h"

#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <png/lodepng.h>


using namespace proto;


static GLuint texName;
static float img_aspect;
static glm::mat4 view;
static GLint uniform_modelview = -1, uniform_projection = -1, uniform_texture = -1;
static unsigned img_width, img_height;



Scene::Scene ()
 : zoom(1.0),
   window_width(640),
   window_height(480)

{
  //Load ();  // move to Setup
  glBufferStorage = (PFNGLBUFFERSTORAGEPROC)glfwGetProcAddress ("glBufferStorage");
  glTexStorage2D = (PFNGLTEXSTORAGE2DPROC)glfwGetProcAddress ("glTexStorage2D");

  if (!glTexStorage2D || !glBufferStorage)
    std::cout << "Problems loading advanced OpenGL API" << std::endl;
}

Scene::~Scene ()
{
  Unload ();
}

void Scene::Load ()
{
 // Load Geometry
 /*
     2------------1
     | \          |
     |   \        |
     |     \      |
     |       \    |
     |          \ |
     3------------0
 */
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

  glBindVertexArray (0);

  // Load texture: load image, generate texture, upload texture
  std::vector<GLubyte> image_data;
  unsigned error = lodepng::decode(image_data, img_width, img_height, "/chattel/cmanning/Pictures/bluespace.png");
  //unsigned error = lodepng::decode(image_data, img_width, img_height, "/chattel/cmanning/Pictures/orion256_mult.png");

  if (error)
    std::cout << "lodepng decode error " << error << ": " << lodepng_error_text(error) << std::endl;
  else
    std::cout << "finished image IO and decode" << std::endl;




  glGenTextures (1, &texName);
  glBindTexture (GL_TEXTURE_2D, texName);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, &image_data[0]);


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
  //GLint uniform_modelview = -1, uniform_projection = -1, uniform_texture = -1;
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

  float fov = 45.0f;  // in degrees, but glm is expecting radians
  float aspect(window_width/window_height);
  img_aspect = float(img_width)/float(img_height);
  float nearz = 0.01f;
  float farz = 10.0f;
  glm::mat4 projection = glm::perspective (glm::radians(fov), aspect, nearz, farz);
  glm::mat4 model = glm::scale (glm::mat4 (1.0f), glm::vec3 (zoom*img_aspect, zoom, zoom));
  //glm::mat4 model = glm::scale (glm::mat4 (1.0f), glm::vec3 (zoom, zoom, zoom));
  float x = tanf(M_PI_2 - glm::radians(fov/2.0f));  // cotangent
  view = glm::lookAt (
      glm::vec3 (0.0f, 0.0f, x),    // camera position in world space
      glm::vec3 (0.0f, 0.0f, 0.0f),  // looking at the origin
      glm::vec3 (0.0f, 1.0f, 0.0f)  // up vector
  );
  glm::mat4 modelView = view * model;



  // use program and set uniforms
  glUseProgram (program);
  glUniformMatrix4fv (uniform_modelview, 1, GL_FALSE, glm::value_ptr(modelView));
  glUniformMatrix4fv (uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
  glUniform1i (uniform_texture, GL_TEXTURE0 - GL_TEXTURE0);
}

void Scene::Unload ()
{ glBindVertexArray (vao);
  glDeleteBuffers (1, &vertex_vbo);
  glDeleteBuffers (1, &index_vbo);
  glDeleteVertexArrays (1, &vao);
  glBindVertexArray (0);

  glDeleteProgram (program);
}

void Scene::Setup ()
{ Load ();
  glClearColor ( 0.0, 0.1, 0.2, 0.0 );
  glEnable (GL_DEPTH_TEST);
  glBindVertexArray (vao);   // onetime bind
}

void Scene::Update ()
{
  // update shader uniforms
  glm::mat4 model = glm::scale (glm::mat4 (1.0f), glm::vec3 (zoom*img_aspect, zoom, zoom));
  glm::mat4 modelView = view * model;
  glUniformMatrix4fv (uniform_modelview, 1, GL_FALSE, glm::value_ptr(modelView));
}

void Scene::Draw ()
{ // bind textures
  glActiveTexture (GL_TEXTURE0);
  glBindTexture (GL_TEXTURE_2D, texName);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDrawElements (GL_TRIANGLE_STRIP, sizeof(index_array)/sizeof(index_array[0]), GL_UNSIGNED_SHORT, 0);
}

