//
// Created by Tristan Zippert on 5/22/21.
//
#pragma once
#include "rcpch.h"

#include "RcEngine/Core/Core.h"
#include "RcEngine/Events/Event.h"

namespace RcEngine{
    struct WindowProps{
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        WindowProps(const std::string& title = "RC-ENGINE",
                    unsigned int width = 1280,
                    unsigned int height = 720)
                    :Title(title),Width(width),Height(height){}
    };
    class RC_API Window{
    public:
        using EventCallbackfn = std::function<void(Event&)>;

        virtual ~Window(){}

        virtual void OnUpdate() = 0;

        virtual unsigned int GetWidth() const =0;
        virtual unsigned int GetHeight() const =0;

        //attributes
        virtual void SetEventCallback(const EventCallbackfn& callback) =0;
        virtual void SetVSync(bool enabled) =0;
        virtual bool IsVsync() const =0;
        static Window* Create(const WindowProps& props =WindowProps());

        virtual void* GetNativeWindow() const = 0;
    };
}
