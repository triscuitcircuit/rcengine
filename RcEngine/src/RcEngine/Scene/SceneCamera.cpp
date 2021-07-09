//
// Created by Tristan Zippert on 7/7/21.
//
#include "rcpch.h"
#include "SceneCamera.h"

#include <external/glm/glm/gtc/matrix_transform.hpp>

namespace RcEngine{
    SceneCamera::SceneCamera() {
        RecalculateProjection();
    }
    void SceneCamera::SetViewPortSize(uint32_t width,
                                      uint32_t height) {
        m_AspectRatio = (float)width / (float)height;
        RecalculateProjection();
    }
    void SceneCamera::SetOrthographic(float size,
                                      float nearClip, float farClip) {
        m_ProjectionType = ProjectionType::Ortho;

        m_OrthographicSize = size;
        m_OrthographicNear = nearClip;
        m_OrthographicFar = farClip;
        RecalculateProjection();

    }
    void SceneCamera::SetIsometric(float size, float nearClip, float farclip) {
        m_ProjectionType = ProjectionType::Iso;

        m_OrthographicSize = size;
        m_OrthographicNear = nearClip;
        m_OrthographicFar = farclip;
        RecalculateProjection();
    }
    void SceneCamera::SetPerspective(float fov, float nearclip, float farclip) {
        m_ProjectionType = ProjectionType::Perspective;
        m_PerspectiveFov = fov;
        m_PerspectiveNear = nearclip;
        m_PerspectiveFar = farclip;

        RecalculateProjection();
    }

    void SceneCamera::RecalculateProjection() {
        if(m_ProjectionType == ProjectionType::Perspective){
            m_Projection = glm::perspective(m_PerspectiveFov, m_AspectRatio, m_PerspectiveNear,m_PerspectiveFar);
        }
        if(m_ProjectionType == ProjectionType::Ortho){
            float orthoLeft= -m_OrthographicSize * m_AspectRatio * 0.5f;
            float orthoRight =m_OrthographicSize * m_AspectRatio * 0.5f;
            float orthoBottom = -m_OrthographicSize * 0.5f;
            float orthoTop = m_OrthographicSize *  0.5f;


            m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom,orthoTop,
                                      m_OrthographicNear, m_OrthographicFar);
        }
        if(m_ProjectionType == ProjectionType::Iso){
            float isoleft= 0;
            float isoright =m_OrthographicSize * m_AspectRatio * 0.5f;
            float isobottom = -m_OrthographicSize * 0.5f;
            float isotop = m_OrthographicSize *  0.5f;

            m_Projection = glm::ortho(isoleft, isoright, isobottom,isotop,
                                      m_IsometricNear, m_IsometricFar);
        }

    }
}

