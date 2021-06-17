//
// Created by Tristan Zippert on 6/6/21.
//
#include "rcpch.h"
#include "OrthoCamera.h"
#include <gtc/matrix_transform.hpp>
namespace RcEngine{
    OrthoCamera::OrthoCamera(float left, float right, float bottom, float top)
    : m_ProjectionMatrix(glm::ortho(left,right,bottom,top,-1.0f, 1.0f)),m_ViewMatrix(1.0f){
        m_ViewProjectMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
    void OrthoCamera::RecalculateView() {

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position)*
                glm::rotate(glm::mat4(1.0f),m_Rotation,
                            glm::vec3(0,0,1));

        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjectMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}
