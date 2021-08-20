//From Cherno's Hazel engine
#include <glm/glm.hpp>

#ifndef RCENGINE_MATH_H
#define RCENGINE_MATH_H
namespace RcEngine::Math{
    bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
    float Qsqrt(const float& x);
}
#endif //RCENGINE_MATH_H
