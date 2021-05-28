//
// Created by Tristan Zippert on 5/21/21.
//
#pragma once

#include "Core.h"
#include "RcEngine/LayerStack.h"
#include "RcEngine/Window.h"
#include "RcEngine/Events/ApplicationEvent.h"

#include "RcEngine/ImGui/ImGuiLayer.h"

#include "RcEngine/Renderer/Shader.h"
#include "RcEngine/Renderer/Buffer.h"

namespace RcEngine{
    class RC_API Application{
    public:
        Application();
        virtual  ~Application();

        void Run();

        void OnEvent(Event& e);
        bool OnWindowClosed(WindowCloseEvent& e);

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

        unsigned int m_VertexArray;

        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
    };
    // Client defined
    Application* CreateApplication();
}
