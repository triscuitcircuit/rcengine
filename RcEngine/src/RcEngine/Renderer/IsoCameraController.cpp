//
// Created by Tristan Zippert on 7/3/21.
//
#include "rcpch.h"

#include "IsoCameraController.h"

#include "RcEngine/Core/Input.h"
#include "RcEngine/Core/keycodes.h"

namespace RcEngine{
    IsoCameraController::IsoCameraController(float aspect, bool rotation)
            : m_AspectRatio(aspect),m_Bounds({-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel,m_ZoomLevel}),
              m_Rotation(rotation){

    }
    void IsoCameraController::OnEvent(Event &e) {
        RC_PROFILE_FUNCTION();
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(
                RC_BIND_EVENT_TO_FUNCTION(IsoCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(
                RC_BIND_EVENT_TO_FUNCTION(IsoCameraController::OnWindowResized));

    }
    void IsoCameraController::OnUpdate(Timestep ts) {
        RC_PROFILE_FUNCTION();
        if(Input::IsKeyPressed(RC_KEY_A)){
            m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) *
                                  m_CameraTranslationSpeed*ts;
        }
        if(Input::IsKeyPressed(RC_KEY_D)){
            m_CameraPosition.x += cos(glm::radians(m_CameraRotation))*
                                  m_CameraTranslationSpeed*ts;
        }
        if(Input::IsKeyPressed(RC_KEY_W)){
            m_CameraPosition.y += m_CameraTranslationSpeed*ts;
        }
        if(Input::IsKeyPressed(RC_KEY_S))
            m_CameraPosition.y -= m_CameraTranslationSpeed*ts;


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
    bool IsoCameraController::OnMouseScrolled(MouseScrolledEvent &e) {
        m_ZoomLevel -= e.GetYOffset() * 0.1f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.04f);
        m_Bounds = {-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel,m_ZoomLevel};
        m_Camera.SetProjection(0,m_Bounds.Right,m_Bounds.Bottom,m_Bounds.Top);
        return false;
    }
    bool IsoCameraController::OnWindowResized(WindowResizeEvent &e) {
        m_AspectRatio = (float)e.GetWidth()/ e.GetHeight();
        m_Camera.SetProjection(0,m_Bounds.Right,m_Bounds.Bottom,m_Bounds.Top);
        return false;
    }


}
