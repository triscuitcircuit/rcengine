//
// Created by Tristan Zippert on 5/22/21.
//


#include "RcEngine/Core/Window.h"
#include "RcEngine/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>
#include <include/glad/glad.h>

namespace RcEngine{
    class MacWindow: public Window{
    public:
        MacWindow(const WindowProps & props);
        virtual ~MacWindow();

        void OnUpdate() override;

        inline unsigned int GetWidth() const override {return m_Data.Width; }
        inline unsigned int GetHeight() const override {return m_Data.Height; }

        //Window attributes

        inline void SetEventCallback(const EventCallbackfn& callback) override {m_Data.EventCallback = callback; }
        void  SetVSync(bool enabled) override;
        bool IsVsync() const override ;

        virtual void* GetNativeWindow() const override{ return m_Window; }
    private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();
    private:
        GLFWwindow* m_Window;
        Scope<GraphicsContext> m_Context;

        struct WindowData{
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackfn EventCallback;
        };
        WindowData m_Data;
    };
}

