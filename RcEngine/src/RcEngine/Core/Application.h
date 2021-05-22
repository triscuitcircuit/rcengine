//
// Created by Tristan Zippert on 5/21/21.
//
#pragma once
#include "Core.h"
#include "RcEngine/Events/Event.h"
namespace RcEngine{
    class RC_API Application{
    public:
        Application();
        virtual  ~Application();

        void Run();
    };
    // Client defined
    Application* CreateApplication();
}
