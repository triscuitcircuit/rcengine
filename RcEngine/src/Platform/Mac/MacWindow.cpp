//
// Created by Tristan Zippert on 5/22/21.
//
#include "MacWindow.h"
#include "rcpch.h"

#include "RcEngine/Events/MouseEvent.h"
#include "RcEngine/Events/ApplicationEvent.h"
#include "RcEngine/Events/KeyEvent.h"

#include <include/glad/glad.h>
//#include "external/GLFW/include/GLFW/glfw3.h"


namespace RcEngine{
    static bool s_GLFWInitialized = false;


    static void GLFWErrorCallback(int error, const char* desc){
        RC_CORE_ERROR("GLFW Error {{0}}: {1}", error, desc);
    }
    Window* Window::Create(const WindowProps &props) {
        return new MacWindow(props);
    }

    MacWindow::MacWindow(const WindowProps &props) {
        Init(props);
    }
    MacWindow::~MacWindow(){}

    void MacWindow::Init(const WindowProps &props) {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        RC_CORE_INFO("Creating window {0} ({1} {2})", props.Title, props.Width, props.Height);

        if (!s_GLFWInitialized){
            int success = glfwInit();
            RC_ASSERT(success, "could not init GLFW");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);


        m_Window = glfwCreateWindow((int)props.Width,(int)props.Height,m_Data.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        RC_CORE_ASSERT(status, "failed to init glad loader");
        glfwSetWindowUserPointer(m_Window,&m_Data);
        // Mac specific call
        SetVSync(true);

        //GLFW callbacks
        glfwSetWindowSizeCallback(m_Window,[](GLFWwindow* window, int width, int height){
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            data.Width = width;
            data.Height = height;
            WindowResizeEvent event(width,height);
            data.EventCallback(event);

        });
        glfwSetWindowCloseCallback(m_Window,[](GLFWwindow* window){
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });
        glfwSetKeyCallback(m_Window,[](GLFWwindow* window, int key, int scancode, int action, int mods){
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            switch (action) {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key,0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key,1);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
            }
        });
        glfwSetCharCallback(m_Window,[](GLFWwindow* window, unsigned int keycode) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            KeyTypedEvent event(keycode);
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods){
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_Window,[](GLFWwindow* window, double xOffset, double yOffset){
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float)xOffset,(float)yOffset);
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window,[](GLFWwindow* window, double xPos, double yPos){
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float )xPos, (float)yPos);
            data.EventCallback(event);
        });
    }

    void MacWindow::Shutdown() {
        glfwDestroyWindow(m_Window);
    }

    void MacWindow::OnUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void MacWindow::SetVSync(bool enabled) {
        if(enabled)
            glfwSwapInterval(1);
    }
    bool MacWindow::IsVsync() const {
        return m_Data.VSync;
    }
}