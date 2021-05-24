//
// Created by Tristan Zippert on 5/21/21.
//
#pragma once
#include "Core.h"
#include "RcEngine/LayerStack.h"
#include "RcEngine/Window.h"
#include "RcEngine/Events/ApplicationEvent.h"

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


        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        LayerStack m_LayerStack;
    };
    // Client defined
    Application* CreateApplication();
}
