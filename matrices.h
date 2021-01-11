#ifndef matrices_h
#define matrices_h

#include </usr/local/include/glm/glm.hpp>

using namespace glm;

mat4 translation(float x, float y, float z);

mat4 rotation(float alpha, float beta, float gamma);

mat4 projection(float aspect_ratio, float FOV, float near, float far);

//mat4 rotation_around(float x, float y, float z);

#endif
