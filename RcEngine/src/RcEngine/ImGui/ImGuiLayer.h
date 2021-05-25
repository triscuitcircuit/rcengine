//
// Created by Tristan Zippert on 5/23/21.
//


#include "RcEngine/Layer.h"


#include "RcEngine/Events/MouseEvent.h"
#include "RcEngine/Events/ApplicationEvent.h"
#include "RcEngine/Events/KeyEvent.h"

namespace RcEngine{
    class RC_API ImGuiLayer : public Layer{

    private:
        float m_Time =0.0f;
    private:
        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
        bool OnMouseMoveEvent(MouseMovedEvent& e);
        bool OnMouseScrolledEvent(MouseScrolledEvent& e);

        bool OnKeyPressedEvent(KeyPressedEvent& e);
        bool OnKeyReleased(KeyReleasedEvent& e);
        bool OnKeyTyped(KeyTypedEvent& e);

        bool OnWindowResize(WindowResizeEvent& e);

    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach();
        void OnDetach();
        void OnUpdate();
        void OnEvent(Event& event);
    };
}