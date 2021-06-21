//
// Created by Tristan Zippert on 6/19/21.
//
#pragma once
#include "RcEngine/Renderer/Texture.h"
#include <include/glad/glad.h>

#ifndef RCENGINE_CLION_OPENGLTEXTURE_H
#define RCENGINE_CLION_OPENGLTEXTURE_H


namespace RcEngine{
    class OpenGLTexture2D: public Texture2D {
    public:
        OpenGLTexture2D(uint32_t width, uint32_t height);
        OpenGLTexture2D(const char* path);

        virtual ~OpenGLTexture2D();

        virtual uint32_t  GetHeight() const override{return m_Height;}
        virtual uint32_t  GetWidth() const override{return m_Width;}
        virtual uint32_t GetRendererID() const override {return m_RendererID;}

        virtual void Bind(uint32_t slot = 0) const override;

    private:
        std::string m_Path;
        uint32_t m_Width, m_Height;
        uint32_t m_RendererID;
        GLenum m_InternalFormat, m_DataFormat;
    };
}


#endif //RCENGINE_CLION_OPENGLTEXTURE_H
