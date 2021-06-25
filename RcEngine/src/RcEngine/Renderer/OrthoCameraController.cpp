//
// Created by Tristan Zippert on 6/23/21.
//
#include "rcpch.h"

#include "OrthoCameraController.h"

#include "RcEngine/Core/Input.h"
#include "RcEngine/Core/keycodes.h"

namespace RcEngine{
    OrthoCameraController::OrthoCameraController(float aspect, bool rotation)
    : m_AspectRatio(aspect), m_Camera(-m_AspectRatio * m_ZoomLevel,m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
    m_Rotation(rotation){

    }
    void OrthoCameraController::OnEvent(Event &e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(
                RC_BIND_EVENT_TO_FUNCTION(OrthoCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(
                RC_BIND_EVENT_TO_FUNCTION(OrthoCameraController::OnWindowResized));

    }
    void OrthoCameraController::OnUpdate(Timestep ts) {
        if(Input::IsKeyPressed(RC_KEY_A)){
            m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) *
                    m_CameraTranslationSpeed*ts;
        }
        if(Input::IsKeyPressed(RC_KEY_D)){
            m_CameraPosition.x += cos(glm::radians(m_CameraRotation))*
                    m_CameraTranslationSpeed*ts;
        }
        if(Input::IsKeyPressed(RC_KEY_W)){
            m_CameraPosition.y -= -sin(glm::radians(m_CameraRotation))*
                    m_CameraTranslationSpeed*ts;
        }
        if(Input::IsKeyPressed(RC_KEY_S))
            m_CameraPosition.y += -sin(glm::radians(m_CameraRotation))*
                                  m_CameraTranslationSpeed*ts;


        if(m_Rotation) {
            if (Input::IsKeyPressed(RC_KEY_LEFT))
                m_CameraRotation -= m_CameraRotationSpeed * ts;
            if (Input::IsKeyPressed(RC_KEY_RIGHT))
                m_CameraRotation += m_CameraRotationSpeed * ts;

            m_Camera.SetRotation(m_CameraRotation);
        }

        m_Camera.SetPosition(m_CameraPosition);
        m_CameraTranslationSpeed = m_ZoomLevel * 1.25f;

    }
    bool OrthoCameraController::OnMouseScrolled(MouseScrolledEvent &e) {
        m_ZoomLevel -= e.GetYOffset() * 0.1f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.04f);
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel,m_AspectRatio * m_ZoomLevel,
                               -m_ZoomLevel, m_ZoomLevel);

        return false;
    }
    bool OrthoCameraController::OnWindowResized(WindowResizeEvent &e) {
        m_AspectRatio = (float)e.GetWidth()/ e.GetHeight();
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel,-m_ZoomLevel, m_ZoomLevel);
        return false;
    }


}