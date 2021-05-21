//
// Created by Tristan Zippert on 5/21/21.
//
#pragma once
#include <RcEngine.h>

#ifdef RC_PLATFORM_WINDOWS
#else
    extern RcEngine::Application* RcEngine::CreateApplication();

    int main(int argc, char** argv){
        auto app = RcEngine::CreateApplication();
        app->Run();
        delete app;
        return 0;
    }
#endif
