//
// Created by Tristan Zippert on 5/26/21.
//
#pragma once
#include <string>

typedef uint32_t RendererID;
namespace RcEngine{

    class Shader{
    public:
        virtual ~Shader() = default;
        virtual void Bind() const =0;
        virtual void Unbind() const =0;

        static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
    };
}
