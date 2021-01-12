#ifndef matrices_h
#define matrices_h

#include <cmath>

#include </usr/local/include/glm/glm.hpp>
#include </usr/local/include/glm/gtx/string_cast.hpp>

glm::mat4 translation(float x, float y, float z);

glm::mat4 rotation(float alpha, float beta, float gamma);

glm::mat4 projection(float aspect_ratio, float FOV, float near, float far);

//glm::mat4 rotation_around(float x, float y, float z);

#endif
