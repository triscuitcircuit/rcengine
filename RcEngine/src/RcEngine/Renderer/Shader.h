//
// Created by Tristan Zippert on 5/26/21.
//
#pragma once
#include <string>
#include <glm.hpp>

typedef uint32_t RendererID;
namespace RcEngine{

    class Shader{
    public:
        Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~Shader();
        void Bind() const;
        void Unbind() const;

        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
private:
        RendererID m_RendererID;
    };
}
