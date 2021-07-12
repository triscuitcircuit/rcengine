//From Cherno's Hazel engine
#include <glm.hpp>

#ifndef RCENGINE_MATH_H
#define RCENGINE_MATH_H
namespace RcEngine::Math{
    bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
}
#endif //RCENGINE_MATH_H
