//
// Created by Tristan Zippert on 5/21/21.
//
#pragma once
//#include "RcEngine.h"
#include "Log.h"

#ifdef RC_PLATFORM_WINDOWS

extern RcEngine::Application* RcEngine::CreateApplication();

    int main(int argc, char** argv){
        RcEngine::Log::Init();

        RC_CORE_WARN("Init log");
        int a =5;
        RC_INFO("Init log from client var:{0}", a);

        auto app = RcEngine::CreateApplication();
        app->Run();
        delete app;
        return 0;
    }
#else
    extern RcEngine::Application* RcEngine::CreateApplication();

    int main(int argc, char** argv){
        RcEngine::Log::Init();

        RC_CORE_WARN("Init log");
        int a =5;
        RC_INFO("Init log from client var:{0}", a);

        auto app = RcEngine::CreateApplication();
        app->Run();
        delete app;
        return 0;
    }
#endif
