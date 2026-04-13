//
// Created by Tristan Zippert on 5/25/21.
//
#pragma once

namespace RcEngine{
    class GraphicsContext{
    public:
        virtual ~GraphicsContext() = default;
        virtual void Init()=0;
        virtual void SwapBuffers() = 0;

        static Scope<GraphicsContext> Create(void* window);
    };
}
