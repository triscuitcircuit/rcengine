//
// Created by Tristan Zippert on 5/26/21.
//
#include "rcpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace RcEngine{
    Ref<Shader> Shader::Create(const std::string& filepath) {
        switch (Renderer::GetAPI()) {
            case RenderAPI::API::None:
                RC_CORE_ASSERT(false, "No Render API");
                return nullptr;
            case RenderAPI::API::OpenGL:
                //TODO: check OpenGL version
                return std::make_shared<OpenGLShader>(filepath);
            case RenderAPI::API::Vulkan:
                RC_CORE_ASSERT(false, "Vulkan not currently supported");
                return nullptr;
        }
        RC_CORE_ASSERT(false, "Shader not defined");
        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
        switch (Renderer::GetAPI()) {
            case RenderAPI::API::None:
                RC_CORE_ASSERT(false, "No Render API");
                return nullptr;
            case RenderAPI::API::OpenGL:
                //TODO: check OpenGL version
                return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
            case RenderAPI::API::Vulkan:
                RC_CORE_ASSERT(false, "Vulkan not currently supported");
                return nullptr;
        }
        RC_CORE_ASSERT(false, "Shader not defined");
        return nullptr;
    }
    void ShaderLibrary::Add(const std::string &name, const Ref <Shader> &shader) {
        RC_CORE_ASSERT(!Exists(name), "Shader already bound");
        m_Shader[name] = shader;
    }
    void ShaderLibrary::Add(const Ref <Shader> &shader) {
        auto& name = shader->GetName();
        Add(name,shader);
    }
    Ref<Shader> ShaderLibrary::Load(const std::string &filepath) {
        auto shader = Shader::Create(filepath);
        Add(shader);
        return shader;
    }
    Ref<Shader> ShaderLibrary::Load(const std::string &name, const std::string &filepath) {
        auto shader = Shader::Create(filepath);
        Add(shader);
        return shader;
    }
    Ref<Shader> ShaderLibrary::Get(const std::string &name) {
        RC_CORE_ASSERT(Exists(name), "Shader already bound");
        return m_Shader[name];
    }
    bool ShaderLibrary::Exists(const std::string &name) {
        return m_Shader.find(name) != m_Shader.end();
    }
}