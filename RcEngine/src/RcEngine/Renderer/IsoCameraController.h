//
// Created by Tristan Zippert on 7/3/21.
//
#pragma once
#include "RcEngine/Core/Timestep.h"
#include "RcEngine/Renderer/OrthoCamera.h"

#include "RcEngine/Events/ApplicationEvent.h"
#include "RcEngine/Events/MouseEvent.h"

#ifndef RCENGINE_CLION_ISOCAMERACONTROLLER_H
#define RCENGINE_CLION_ISOCAMERACONTROLLER_H
namespace RcEngine{
    struct IsoCameraBounds{
        float Left, Right;
        float Bottom,Top;

        float GetWidth(){return Right - Left;}
        float GetHeight(){return Top-Bottom;}
    };

    class IsoCameraController{
    public:
        IsoCameraController(float aspect, bool rotation= false);

        OrthoCamera& GetCamera(){return m_Camera;}
        const OrthoCamera& GetCamera() const {return m_Camera;}

        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);

        void SetRotationSpeed(float speed){m_CameraRotationSpeed = speed;}
        float GetRotationSpeed(){return m_CameraRotationSpeed;}

        float GetZoomlevel() const {return m_ZoomLevel;}
        void SetZoomlevel(float level){m_ZoomLevel = level;}

        const IsoCameraBounds& GetBounds() const{return m_Bounds;}
    private:
        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);
    private:
        float m_AspectRatio;
        float m_ZoomLevel = 1.0f;

        OrthoCamera m_Camera;

        bool m_Rotation;
        glm::vec3 m_CameraPosition = {0.0f,0.0f,0.0f};
        float m_CameraRotation = 1.0f; //18.0f before

        float m_CameraTranslationSpeed =5.0f, m_CameraRotationSpeed = 9.0f;

        IsoCameraBounds m_Bounds;

    };
}
#endif //RCENGINE_CLION_ISOCAMERACONTROLLER_H
