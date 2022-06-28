//
// Created by Tristan Zippert on 5/21/21.
//

#include "rcpch.h"
#include "Application.h"
#include "RcEngine/Sound/SoundDevice.h"
#include "Log.h"
#include <glad/glad.h>
#include "RcEngine/Renderer/Renderer.h"
#include "RcEngine/Network/Network.h"

#include "Input.h"
#include <../external/GLFW/include/GLFW/glfw3.h>


namespace RcEngine{
#define BIND_EVENT_FN(x) std::bind(&x,this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;
    Application::Application(const std::string& name)
{
        RC_PROFILE_FUNCTION();

        RC_CORE_ASSERT(!s_Instance,"Application already open");
        s_Instance = this;

        m_Window = Window::Create(WindowProps(name));
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
        m_Window->SetVSync(false);

        SoundDevice::Init();
        Renderer::Init();

        //Renderer::SetAPI(RenderAPI::API::Vulkan);

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }
    Application::~Application() {

    }
    void Application::PushLayer(Layer *layer) {
        RC_PROFILE_FUNCTION();

        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }
    void Application::PushOverlay(Layer *layer) {
        RC_PROFILE_FUNCTION();

        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }
    void Application::OnEvent(Event& e){
        RC_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClosed));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

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
    bool Application::OnWindowResize(WindowResizeEvent &e) {

        if(e.GetWidth() == 0|| e.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }
        m_Minimized = false;

        // Windows specific
        //Renderer::OnWindowResize(e.GetHeight(), e.GetHeight());

        return false;
    }
    void Application::Close() {
        m_Running = false;
    }

    void Application::Run() {
        RC_PROFILE_FUNCTION();
        while (m_Running){
            RC_PROFILE_SCOPE("Application Run Loop");
            float time = (float)glfwGetTime();
            Timestep timestep = time- m_LastFrameTime;
            m_LastFrameTime = time;

            if(!m_Minimized){
                {
                    RC_PROFILE_SCOPE("LayerStack OnUpdate");
                    for (Layer *layer: m_LayerStack)
                        layer->OnUpdate(timestep);
                }
                m_ImGuiLayer->Begin();
                {
                    RC_PROFILE_SCOPE("LayerStack OnImGUIRender");
                    for (Layer *layer: m_LayerStack)
                        layer->OnImGuiRender();
                }
                m_ImGuiLayer->End();
            }
            m_Window->OnUpdate();
        }
    }
}