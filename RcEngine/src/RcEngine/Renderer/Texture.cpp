//
// Created by Tristan Zippert on 6/19/21.
//

#include "rcpch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace RcEngine{
    Ref<Texture2D> Texture2D::Create(const char* path) {
        switch (Renderer::GetAPI()) {
            case RenderAPI::API::None:
                RC_CORE_ASSERT(false, "No Render API");
                return nullptr;
            case RenderAPI::API::OpenGL:
                //TODO: check OpenGL version
                return std::make_shared<OpenGLTexture2D>(path);
            case RenderAPI::API::Vulkan:
                RC_CORE_ASSERT(false, "Vulkan not currently supported");
                return nullptr;
        }
        RC_CORE_ASSERT(false, "Renderer not defined");
        return nullptr;
    }
    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
        switch (Renderer::GetAPI()) {
            case RenderAPI::API::None:
                RC_CORE_ASSERT(false, "No Render API");
                return nullptr;
            case RenderAPI::API::OpenGL:
                //TODO: check OpenGL version
                return std::make_shared<OpenGLTexture2D>(width,height);
            case RenderAPI::API::Vulkan:
                RC_CORE_ASSERT(false, "Vulkan not currently supported");
                return nullptr;
        }
        RC_CORE_ASSERT(false, "Renderer not defined");
        return nullptr;
    }
}
