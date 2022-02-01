//
// Created by Tristan Zippert on 6/24/21.
//
#pragma once
#ifndef RCENGINE_CLION_PROJECTIONCAMERA_H
#define RCENGINE_CLION_PROJECTIONCAMERA_H
#include "rcpch.h"
#include "Camera.h"

namespace RcEngine{
    class ProjectionCamera : public Camera{
    public:
        ProjectionCamera() = default;
        ProjectionCamera(float fov, float aspectRatio, float nearClip, float FarClip);

        void SetProjection(float fov, float aspectRatio, float nearClip, float FarClip);


        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }


        float GetPitch() const { return m_Pitch; }
        float GetYaw() const { return m_Yaw; }

        void SetWidthHeight();

        void setRotationSpeed(float speed);
        float RotationSpeed() const {return m_RotationSpeed;}


        glm::vec3 GetUpDirection() const;
        glm::vec3 GetRightDirection() const;
        glm::vec3 GetForwardDirection() const;
        const glm::vec3& GetPosition() const { return m_CameraPosition; }
        glm::quat GetOrientation() const;


    private:
        void RecalculateView();
        glm::vec3 CalculatePosition() const;
        void Rotate(const glm::vec2& delta);

        void UpdateView();

    private:
        float m_FOV = 1.04724f,m_AspectRatio = 1.778f;
        float m_NearClip = 0.1f, m_FarClip = 1000.0f;

        glm::mat4 m_ViewMatrix;

        glm::vec3 m_CameraPosition = {0.0f,0.0f,0.0f};
        glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

        float m_Pitch = 0.0f, m_Yaw = 0.0f;
        float m_Width = 1280, m_Height = 720;

        float m_Distance = 10.0f;
        float m_RotationSpeed = 0.8f;

    };
}
#endif //RCENGINE_CLION_PROJECTIONCAMERA_H
