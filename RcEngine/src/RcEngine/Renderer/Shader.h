//
// Created by Tristan Zippert on 5/26/21.
//
#pragma once
#include <string>

#define RendererID uint32_t

namespace RcEngine{
    class Shader{
    public:
        Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~Shader();
        void Bind() const;
        void Unbind() const;
private:
        RendererID m_RendererID;
};
}
