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
#include "RcEngine/Renderer/VertexArray.h"

#include "RcEngine/Renderer/OrthoCamera.h"

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

        //unsigned int m_VertexArray;

        std::shared_ptr<VertexArray> m_VertexArray;

        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;

        std::shared_ptr<VertexArray> m_SquareVA;

        OrthoCamera m_Camera;
    };
    // Client defined
    Application* CreateApplication();
}
