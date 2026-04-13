//
// Created by Tristan Zippert on 7/6/21.
//
#include "rcpch.h"
#include "RcEngine/Core/Window.h"

#ifdef RC_PLATFORM_WINDOW
    #include "Platform/Windows/WindowsWindow.h"
#endif

#ifdef RC_PLATFORM_MAC
    #include "Platform/Mac/MacWindow.h"
#endif

namespace RcEngine{
    Scope<Window> Window::Create(const WindowProps &props) {
        #ifdef RC_PLATFORM_WINDOWS
                return CreateScope<WindowsWindow>(props);
        #endif

        #ifdef RC_PLATFORM_MAC
            return CreateScope<MacWindow>(props);
        #endif

            RC_CORE_ASSERT(false, "Unknown platform!");
            return nullptr;
    }
}