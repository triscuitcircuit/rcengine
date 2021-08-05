//
// Created by Tristan Zippert on 8/2/21.
//
#include "rcpch.h"
#include "UniformBuffer.h"

#include "RcEngine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace RcEngine{

    Ref <UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding) {
        switch (Renderer::GetAPI()){
            case RenderAPI::API::None: RC_CORE_ASSERT(false,"No Renderer API defined");
                return nullptr;
            case RenderAPI::API::OpenGL: return CreateRef<OpenGlUniformBuffer>(size,binding);
            case RenderAPI::API::Vulkan: RC_CORE_ASSERT(false,"Vulkan not implemented");
                return nullptr;
            case RenderAPI::API::Metal:RC_CORE_ASSERT(false,"Metal not implemented");
                return nullptr;
        }
        RC_CORE_ASSERT(false, "Unknown RendererAPI ");
        return nullptr;
    }
}
