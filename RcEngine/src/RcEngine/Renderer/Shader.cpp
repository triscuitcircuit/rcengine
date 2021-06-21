//
// Created by Tristan Zippert on 5/26/21.
//
#include "rcpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace RcEngine{

    Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {
        switch (Renderer::GetAPI()) {
            case RenderAPI::API::None:
                RC_CORE_ASSERT(false, "No Render API");
                return nullptr;
            case RenderAPI::API::OpenGL:
                //TODO: check OpenGL version
                return new OpenGLShader(vertexSrc, fragmentSrc);
            case RenderAPI::API::Vulkan:
                RC_CORE_ASSERT(false, "Vulkan not currently supported");
                return nullptr;
        }
        RC_CORE_ASSERT(false, "Shader not defined");
        return nullptr;
    }
}