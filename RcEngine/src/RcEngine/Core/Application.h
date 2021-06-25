//
// Created by Tristan Zippert on 5/21/21.
//
#pragma once

#include "Core.h"
#include "LayerStack.h"
#include "Window.h"
#include "RcEngine/Events/ApplicationEvent.h"

#include "RcEngine/ImGui/ImGuiLayer.h"

#include "RcEngine/Renderer/Shader.h"
#include "RcEngine/Renderer/Buffer.h"
#include "RcEngine/Renderer/VertexArray.h"

#include "RcEngine/Renderer/OrthoCamera.h"

#include "RcEngine/Core/Timestep.h"

namespace RcEngine{
    class RC_API Application{
    public:
        Application();
        virtual  ~Application();

        void Run();

        void OnEvent(Event& e);
        bool OnWindowClosed(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e );

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        inline static Application& Get() {return *s_Instance;}
        inline Window& GetWindow(){return *m_Window;}
    private:
        static Application* s_Instance;

        ImGuiLayer* m_ImGuiLayer;

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        LayerStack m_LayerStack;
        bool m_Minimized = false;
        float m_LastFrameTime= 0.0f;


    };
    // Client defined
    Application* CreateApplication();
}
