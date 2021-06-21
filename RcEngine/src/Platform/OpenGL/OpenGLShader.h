//
// Created by Tristan Zippert on 6/17/21.
//
#pragma once
#include "RcEngine/Renderer/Shader.h"
#include <glm.hpp>

#ifndef RCENGINE_CLION_OPENGLSHADER_H
#define RCENGINE_CLION_OPENGLSHADER_H

typedef uint32_t RendererID;
namespace RcEngine{

    class OpenGLShader:public Shader{
    public:
        OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);

        virtual ~OpenGLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        void UploadUniformInt(const std::string& name, const int value);

        void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

        void UploadUniformFloat(const std::string& name, const float values);
        void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
        void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
        void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

        void UploadSampler(const char* name);
    private:
        RendererID m_RendererID;
    };
}

#endif //RCENGINE_CLION_OPENGLSHADER_H
