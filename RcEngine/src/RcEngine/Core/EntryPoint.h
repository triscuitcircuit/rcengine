//
// Created by Tristan Zippert on 5/21/21.
//
#pragma once
#include "RcEngine.h"
#include "RcEngine/Core/Log.h"

#ifdef RC_PLATFORM_WINDOWS
extern RcEngine::Application* RcEngine::CreateApplication();

    int main(int argc, char** argv){
        RcEngine::Log::Init();
        RC_CORE_WARN("Init log");
            RC_PROFILE_BEGIN_SESSION("Startup","RCENGINE-Startup.json");
            auto app = RcEngine::CreateApplication();
        RC_PROFILE_END_SESSION();
        RC_PROFILE_BEGIN_SESSION("Runtime","RCENGINE-runtime.json");
            app->Run();
        RC_PROFILE_END_SESSION();
        RC_PROFILE_BEGIN_SESSION("Shutdown","RCENGINE-Shutdown.json");
            delete app;
        RC_PROFILE_END_SESSION();
        return 0;
    }
#else
    extern RcEngine::Application* RcEngine::CreateApplication();

    int main(int argc, char** argv){
        RcEngine::Log::Init();
        RC_CORE_WARN("Init log");
        RC_PROFILE_BEGIN_SESSION("Startup","RCENGINE-Startup.json");
            auto app = RcEngine::CreateApplication();
        RC_PROFILE_END_SESSION();
        RC_PROFILE_BEGIN_SESSION("Runtime","RCENGINE-runtime.json");
            app->Run();
        RC_PROFILE_END_SESSION();
        RC_PROFILE_BEGIN_SESSION("Shutdown","RCENGINE-Shutdown.json");
            delete app;
        RC_PROFILE_END_SESSION();
        return 0;
    }
#endif
