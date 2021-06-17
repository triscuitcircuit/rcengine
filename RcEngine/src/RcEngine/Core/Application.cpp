//
// Created by Tristan Zippert on 5/21/21.
//

extern "C" void rc_tcp_server();
#include "rcpch.h"
#include "Application.h"
#include "RcEngine/Log.h"
#include "include/glad/glad.h"
#include "RcEngine/Renderer/Renderer.h"

#include "RcEngine/Input.h"
#include <external/GLFW/include/GLFW/glfw3.h>


namespace RcEngine{
#define BIND_EVENT_FN(x) std::bind(&x,this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;
    Application::Application()
{
        rc_tcp_server();

        RC_CORE_ASSERT(!s_Instance,"Application already open");
        s_Instance = this;
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
        m_Window->SetVSync(false);


        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
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
            float time = (float)glfwGetTime();
            Timestep timestep = time- m_LastFrameTime;
            m_LastFrameTime = time;

            for(Layer* layer: m_LayerStack)
                layer->OnUpdate(timestep);

            m_ImGuiLayer->Begin();
            for(Layer* layer: m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }
}