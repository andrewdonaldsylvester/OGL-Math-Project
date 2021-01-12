#ifndef shader_loading_hpp
#define shader_loading_hpp

#include <string>
#include <iostream>
#include <fstream>

#define GL_SILENCE_DEPRECATION

#include </usr/local/include/GLFW/glfw3.h>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif

GLuint loadShaders(const char* vertex_path, const char* fragment_path);

#endif
