//
// Created by Tristan Zippert on 7/5/21.
//

#include "FrameBuffer.h"
#include "rcpch.h"

#include "RcEngine/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace RcEngine{
    Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpec &spec) {
        switch (Renderer::GetAPI()) {
            case RenderAPI::API::None:
                RC_CORE_ASSERT(false, "No Render API")
                return nullptr;
            case RenderAPI::API::OpenGL:
                //TODO: check OpenGL version
                return std::make_shared<OpenGLFramebuffer>(spec);
            case RenderAPI::API::Vulkan:
                RC_CORE_ASSERT(false, "Vulkan not currently supported");
                return nullptr;
        }
        RC_CORE_ASSERT(false, "Renderer not defined");
        return nullptr;
    }
}