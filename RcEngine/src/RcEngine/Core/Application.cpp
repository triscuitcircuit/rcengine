//
// Created by Tristan Zippert on 5/21/21.
//

#include "Application.h"
#include "RcEngine/Log.h"
#include <GLFW/glfw3.h>

namespace RcEngine{
#define BIND_EVENT_FN(x) std::bind(&x,this, std::placeholders::_1)
    Application::Application() {
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
    }
    Application::~Application() {

    }
    void Application::OnEvent(Event& e){
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClosed));

        RC_CORE_INFO("{0}",e);
    }
    bool Application::OnWindowClosed(WindowCloseEvent &e) {
        m_Running = false;
        return true;
    }
    void Application::Run() {
        while (m_Running){
            glClearColor(0.5f,0.0f,0.5f,0);
            glClear(GL_COLOR_BUFFER_BIT);
            m_Window->OnUpdate();
        }
    }
}