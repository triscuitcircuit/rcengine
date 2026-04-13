//
// Created by Tristan Zippert on 5/26/21.
//
#include "rcpch.h"
#include "RcEngine/Renderer/GraphicsContext.h"

#include "RcEngine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace RcEngine{
    Scope<GraphicsContext> GraphicsContext::Create(void* window)
    {
        switch (Renderer::GetAPI())
        {
            case RenderAPI::API::None:    RC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RenderAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
        }

        RC_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}

