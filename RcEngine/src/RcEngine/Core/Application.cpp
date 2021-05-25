//
// Created by Tristan Zippert on 5/21/21.
//

#include "Application.h"
#include "RcEngine/Log.h"
#include <include/glad/glad.h>

#include "external/glm/glm/glm.hpp"

#include "RcEngine/Input.h"

namespace RcEngine{
#define BIND_EVENT_FN(x) std::bind(&x,this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;
    Application::Application() {

        RC_CORE_ASSERT(!s_Instance,"Application already open");
        s_Instance = this;
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));


    }
    Application::~Application() {

    }
    void Application::PushLayer(Layer *layer) {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }
    void Application::PushOverlay(Layer *layer) {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }
    void Application::OnEvent(Event& e){

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClosed));

        RC_CORE_INFO("{0}",e);

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
            (*--it)->OnEvent(e);
            if(e.m_Handeled)
                break;
        }
    }
    bool Application::OnWindowClosed(WindowCloseEvent &e) {
        m_Running = false;
        return true;
    }
    void Application::Run() {
        while (m_Running){
            glClearColor(0.5f,0.0f,0.5f,0);
            glClear(GL_COLOR_BUFFER_BIT);

            for(Layer* layer: m_LayerStack)
                layer->OnUpdate();

//            std::tuple<float, float> tuple = Input::GetMousePos();
//            RC_CORE_TRACE("{0} {1}", std::get<0>(tuple), std::get<1>(tuple));

            m_Window->OnUpdate();
        }
    }
}