//
// Created by Tristan Zippert on 7/7/21.
//
#pragma once
#include "RcEngine/Renderer/Camera.h"


#ifndef RCENGINE_CLION_SCENECAMERA_H
#define RCENGINE_CLION_SCENECAMERA_H
namespace RcEngine{
    class SceneCamera: public Camera{
    public:
        enum class ProjectionType {Perspective =0, Ortho =1, Iso =2};
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void SetOrthographic(float size, float nearClip, float farClip);
        void SetPerspective(float fov, float nearclip, float farclip);
        void SetIsometric(float size, float nearClip, float farclip);

        void SetViewPortSize(uint32_t width, uint32_t height);

        float GetOrthographicSize() const {return m_OrthographicSize;}
        void SetOrthographicSize(float size) {m_OrthographicSize = size; RecalculateProjection();}
        float GetOrthoNearClip() const{return m_OrthographicNear;}
        float GetOrthoFarClip() const{return m_OrthographicFar;}
        void SetOrthoNearClip(float nearclip){m_OrthographicNear = nearclip;RecalculateProjection();}
        void SetOrthoFarClip(float farclip){m_OrthographicFar = farclip;RecalculateProjection();}


        float GetPerspectiveVerticalFov() const {return m_PerspectiveFov;}
        void SetPerspectiveVerticalFov(float fov) {m_PerspectiveFov = fov; RecalculateProjection();}
        float GetPerspectiveNearClip() const{return m_PerspectiveNear;}
        float GetPerspectiveFarClip() const{return m_PerspectiveFar;}
        void SetPerspectiveNearClip(float nearclip){m_PerspectiveNear = nearclip;RecalculateProjection();}
        void SetPerspectiveFarClip(float farclip){m_PerspectiveFar = farclip;RecalculateProjection();}

        float GetIsoSize() const {return m_IsoSize;}
        void SetIsoSize(float size) {m_IsoSize = size; RecalculateProjection();}
        float GetIsoNearClip() const{return m_IsometricNear;}
        float GetIsoFarClip() const{return m_IsometricFar;}
        void SetIsoNearClip(float nearclip){m_IsometricNear = nearclip;RecalculateProjection();}
        void SetIsoFarClip(float farclip){m_IsometricFar = farclip;RecalculateProjection();}


        ProjectionType GetProjectionType() const {return m_ProjectionType;}
        void SetProjectionType(ProjectionType type){m_ProjectionType = type;}
    private:
        void RecalculateProjection();
    private:
        ProjectionType m_ProjectionType = ProjectionType::Ortho;

        float m_OrthographicSize = 10.0f;
        float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

        float m_PerspectiveFov = glm::radians(45.0f);
        float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

        float m_IsoSize = 10.0f;
        float m_IsometricNear = -1.0f, m_IsometricFar = 1.0f;

        float m_AspectRatio =0.0f;
    };
}
#endif //RCENGINE_CLION_SCENECAMERA_H
