
#ifndef __Util__proto__
#define __Util__proto__

#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>


#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX 0x9049
#define GPU_MEMORY_INFO_EVICTED_MEMORY_NVX 0x904B
#ifndef GL_TEXTURE_FREE_MEMORY_ATI
#define GL_TEXTURE_FREE_MEMORY_ATI 0x87FC
#endif

namespace proto {

class Util {

public:
static bool compileShader (GLuint &shader, GLenum type,
                             const char *src)
{ shader = glCreateShader (type);
  glShaderSource (shader, 1, &src, NULL);
  glCompileShader (shader);

#if defined(DEBUG)
  GLint logLength;
  glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 0) {
      std::vector<GLchar> plog(logLength);
      glGetShaderInfoLog (shader, logLength, &logLength, &plog[0]);
      std::string strLog(&plog[0], &plog[0] + logLength);
      std::cout << strLog << std::endl;
  }
#endif

  GLint status;
  glGetShaderiv (shader, GL_COMPILE_STATUS, &status);
  if (status == 0) {
      glDeleteShader (shader);
      return false;
  }
  return true;
}


static bool linkProgram (GLuint &program)
{ glLinkProgram (program);

#if defined(DEBUG)
  GLint logLength;
  glGetProgramiv (program, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 0) {
      std::vector<GLchar> plog(logLength);
      glGetProgramInfoLog (program, logLength, &logLength, &plog[0]);
      std::string strLog(&plog[0], &plog[0] + logLength);
      std::cout << strLog << std::endl;
  }
#endif

  GLint status;
  glGetProgramiv (program, GL_LINK_STATUS, &status);
  if (status == 0) {
      return false;
  }
  return true;
}

static std::string loadShaderSourceFromFile (const char *filepath)
{ std::ifstream in (filepath);
  std::stringstream ss;
  ss << in.rdbuf ();
  return ss.str ();
}


static void query_gpu_mem_usage_nvidia()
{
  GLint minfo;
  glGetIntegerv(GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &minfo);
  GLint evicted;
  glGetIntegerv(GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &evicted);
  printf("GL nvidia total available memory %d MB  (evicted memory %d) \n", minfo/1024, evicted/1024);
}

static void query_gpu_mem_usage_amd()
{
  GLint minfo[4];
  glGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI, minfo);
  printf("GL amd total free memory in pool %d MB\n", minfo[0]/1024);
}

static void query_memory()
{
  std::string vendor_string ( reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
  std::cout << vendor_string << std::endl;
  if (vendor_string . find ("nvidia") != std::string::npos ||
      vendor_string . find ("NVIDIA") != std::string::npos)
    query_gpu_mem_usage_nvidia ();
  else if (vendor_string . find ("ati") != std::string::npos ||
           vendor_string . find ("amd") != std::string::npos)
    query_gpu_mem_usage_amd ();
  else
    printf("unsupported graphics card vendor\n");
}


};

}

#endif // defined(__Scene__proto__)
