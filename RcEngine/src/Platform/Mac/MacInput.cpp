//
// Created by Tristan Zippert on 5/24/21.
//

#include "MacInput.h"
#include "rcpch.h"
#include "RcEngine/Core/Application.h"

#include <GLFW/glfw3.h>

namespace RcEngine{

    Input* Input::s_Instance  = new MacInput();

    bool MacInput::IsKeyPressedImpl(int keycode) {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

        auto state = glfwGetKey(window, keycode);

        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    float MacInput::GetMouseXImpl() {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xpos, ypos = 0.0;
        glfwGetCursorPos(window,&xpos,&ypos);
        return (float)xpos;
    }
    float MacInput::GetMouseYImpl() {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xpos, ypos = 0.0;
        glfwGetCursorPos(window,&xpos,&ypos);
        return (float)ypos;
    }
    std::pair<float,float> MacInput::GetMousePosImpl() {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xpos, ypos = 0.0;
        glfwGetCursorPos(window,&xpos,&ypos);
        return {(float)xpos, (float)ypos};
    }
    bool MacInput::IsMouseButtonPressedImpl(int button) {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }
}

