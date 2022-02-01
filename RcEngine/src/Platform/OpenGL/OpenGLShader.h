//
// Created by Tristan Zippert on 6/17/21.
//
#pragma once
#include "RcEngine/Renderer/Shader.h"
#include <glm/glm.hpp>
//#include <include/glad/glad.h>
typedef unsigned int GLenum;

#ifndef RCENGINE_CLION_OPENGLSHADER_H
#define RCENGINE_CLION_OPENGLSHADER_H

typedef uint32_t RendererID;
namespace RcEngine{

    class OpenGLShader:public Shader{
    public:
        OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        OpenGLShader(const std::string& filepath);

        virtual ~OpenGLShader();

        virtual const std::string& GetName() const override{
            return m_Name;
        };

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void SetFloat(const std::string& name, const float value) override;
        virtual void SetInt(const std::string& name, const int value) override;
        virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;

        virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
        virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
        virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

        void UploadUniformInt(const std::string& name, const int value);
        void UploadUniformIntArray(const std::string &name, int* values, uint32_t count);

        void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

        void UploadUniformFloat(const std::string& name, const float values);
        void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
        void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
        void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

        void UploadSampler(const char* name);
    private:
        std::string ReadFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSource);
    private:
        RendererID m_RendererID;
        std::string m_Name;
    };
}

#endif //RCENGINE_CLION_OPENGLSHADER_H
