//
// Created by Tristan Zippert on 5/22/21.
//
#include "MacWindow.h"
#include "rcpch.h"

#include "RcEngine/Events/MouseEvent.h"
#include "RcEngine/Events/ApplicationEvent.h"
#include "RcEngine/Events/KeyEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"


namespace RcEngine{
    static bool s_GLFWInitialized = false;
    static uint8_t s_GLFW_Win_Count = 0;


    static void GLFWErrorCallback(int error, const char* desc){
        RC_CORE_ERROR("GLFW Error {{0}}: {1}", error, desc);
    }


    MacWindow::MacWindow(const WindowProps &props) {
        RC_PROFILE_FUNCTION();
        Init(props);
    }
    MacWindow::~MacWindow(){
        RC_PROFILE_FUNCTION();

        Shutdown();
    }

    void MacWindow::Init(const WindowProps &props) {
        RC_PROFILE_FUNCTION();

        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;


        RC_CORE_INFO("Creating window {0} ({1} {2})", props.Title, props.Width, props.Height);

        if (!s_GLFWInitialized){
            RC_PROFILE_SCOPE("glfwCreateWindow");

            int success = glfwInit();
            RC_ASSERT(success, "could not init GLFW");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        {
            ++s_GLFW_Win_Count;
        }

        int actualScreenWidth, actualScreenHeight;

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);



        m_Window = glfwCreateWindow((int)props.Width,(int)props.Height,m_Data.Title.c_str(), nullptr, nullptr);

        m_Context = GraphicsContext::Create(m_Window);
        m_Context->Init();

        glfwGetFramebufferSize(m_Window,&actualScreenWidth,&actualScreenHeight);


        glViewport(0,0,actualScreenWidth,actualScreenHeight);

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
        RC_PROFILE_FUNCTION();
        glfwDestroyWindow(m_Window);
        --s_GLFW_Win_Count;

        if(s_GLFW_Win_Count)
            glfwTerminate();
    }

    void MacWindow::OnUpdate() {
        glfwPollEvents();
        m_Context->SwapBuffers();
    }

    void MacWindow::SetVSync(bool enabled) {
        glfwSwapInterval((int)enabled);
        m_Data.VSync = enabled;
    }
    bool MacWindow::IsVsync() const {
        return m_Data.VSync;
    }
}