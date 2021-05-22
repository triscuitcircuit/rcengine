//
// Created by Tristan Zippert on 5/21/21.
//
#include <RcEngine/Events/ApplicationEvent.h>
#include "Application.h"

namespace RcEngine{
    Application::Application() {}
    Application::~Application() {}
    void Application::Run() {
        WindowResizeEvent e(1260,720);
        RC_TRACE(e);
        while (true){} }
}