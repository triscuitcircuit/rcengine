//
// Created by Tristan Zippert on 6/6/21.
//
#pragma once
#include <glm.hpp>


#ifndef RCENGINE_CLION_ORTHOCAMERA_H
#define RCENGINE_CLION_ORTHOCAMERA_H
namespace RcEngine{
    class OrthoCamera{
    public:
        OrthoCamera(float left=-1.0f, float right=-1.0f, float bottom=-1.0f, float top=-1.0f);
        const glm::vec3& GetPosition() const{return m_Position;}


        void SetPosition(const glm::vec3& position){m_Position = position;}


        float GetRotation() const {return m_Rotation;}

        void SetRotation(float rotation){
            m_Rotation = rotation;
            RecalculateView();
        }
    public:
        const::glm::mat4& GetProjectionMatrix() const {return m_ProjectionMatrix;}
        const glm::mat4& GetViewMatrix() const{return m_ViewMatrix;}
        const glm::mat4& GetViewProjectMatrix() const{return m_ViewProjectMatrix;}
    private:
        void RecalculateView();

    private:
    public:
        glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
    private:
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectMatrix;

        glm::vec3 m_Position = {0.0f,0.0f,0.0f};
        float m_Rotation = 0.0f;
    };
}
#endif //RCENGINE_CLION_ORTHOCAMERA_H
