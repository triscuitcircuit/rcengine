//
// Created by Tristan Zippert on 5/25/21.
//
#include "rcpch.h"
#include "OpenGLContext.h"


#include <../external/GLFW/include/GLFW/glfw3.h>
#include <../include/glad/glad.h>

namespace RcEngine{
    OpenGLContext::OpenGLContext(GLFWwindow *windowHandle)
        :m_WindowHandle(windowHandle)
    {
        RC_CORE_ASSERT(m_WindowHandle, "Window is null");

    }
    void OpenGLContext::Init() {
        RC_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        RC_CORE_ASSERT(status, "failed to init glad loader");

        RC_CORE_INFO(" Opengl Renderer: {0}",glGetString(GL_VENDOR));
        RC_CORE_INFO(" Vendor {0}", glGetString(GL_VERSION));
    }
    void OpenGLContext::SwapBuffers() {
        RC_PROFILE_FUNCTION();
        glfwSwapBuffers(m_WindowHandle);
    }
}
