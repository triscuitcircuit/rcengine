//
// Created by Tristan Zippert on 6/24/21.
//
#include "ProjectionCamera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace RcEngine{
        ProjectionCamera::ProjectionCamera(float fov, float aspectRatio,
                                           float nearClip, float FarClip)
            : m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(FarClip)
            , Camera(
                    glm::perspective(glm::radians(fov), aspectRatio, nearClip, FarClip)){
                UpdateView();
            }

            void ProjectionCamera::SetProjection(float fov, float aspectRatio, float nearClip, float FarClip)
            {
                m_FOV = fov;
                m_AspectRatio = m_Width / m_Height;
                m_NearClip = nearClip;
                m_FarClip = FarClip;
                m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);

            }

            void ProjectionCamera::setRotationSpeed(float speed)  {
                    m_RotationSpeed = speed;
             }

            void ProjectionCamera::UpdateView() {
                m_CameraPosition = CalculatePosition();

                glm::quat orientation = GetOrientation();
                m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_CameraPosition) * glm::toMat4(orientation);
                m_ViewMatrix = glm::inverse(m_ViewMatrix);
            }



        glm::vec3 ProjectionCamera::GetUpDirection() const
        {
            return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
        }

        glm::vec3 ProjectionCamera::GetRightDirection() const
        {
            return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
        }

        glm::vec3 ProjectionCamera::GetForwardDirection() const
        {
            return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
        }

        glm::vec3 ProjectionCamera::CalculatePosition() const
        {
            return m_FocalPoint - GetForwardDirection() * m_Distance;
        }

        glm::quat ProjectionCamera::GetOrientation() const
        {
            return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
        }

        void ProjectionCamera::RecalculateView() {
            m_AspectRatio = m_Width/m_Height;
            m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio,m_NearClip,m_FarClip);
        }
        void ProjectionCamera::Rotate(const glm::vec2 &delta) {
            float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
            m_Yaw += yawSign * delta.x * RotationSpeed();
        }
    }