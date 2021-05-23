//
// Created by Tristan Zippert on 5/21/21.
//
#pragma once
#include "Core.h"
#include "RcEngine/Events/ApplicationEvent.h"
#include "RcEngine/Window.h"

namespace RcEngine{
    class RC_API Application{
    public:
        Application();
        virtual  ~Application();

        void Run();

        void OnEvent(Event& e);
        bool OnWindowClosed(WindowCloseEvent& e);
    private:

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };
    // Client defined
    Application* CreateApplication();
}
