//
// Created by Tristan Zippert on 6/19/21.
//
#include "rcpch.h"
#include "OpenGLTexture.h"
#include <SOIL2.h>
#include <iostream>
#include <filesystem>


namespace RcEngine{
    OpenGLTexture2D::OpenGLTexture2D(const char* path)
    :m_Path(path){
        RC_PROFILE_FUNCTION();
        int width,channels,height;
        unsigned char* data = nullptr;
        {
            RC_PROFILE_SCOPE("SOIL Load - OpenGLTexture2D::OpenGLTexture2D(const char* path)");
            data = SOIL_load_image(path, &width, &height, &channels, SOIL_LOAD_AUTO);
            m_RendererID = SOIL_create_OGL_texture(data, &width, &height, channels, SOIL_CREATE_NEW_ID,
                                                   SOIL_FLAG_INVERT_Y | SOIL_FLAG_GL_MIPMAPS);
        }
        GLenum internalFormat =0, dataformat =0;
        if(channels == 4){
            internalFormat = GL_RGBA8;
            dataformat = GL_RGBA;
        }else if(channels ==3){
            internalFormat = GL_RGB8;
            dataformat = GL_RGB;
        }

        m_InternalFormat = internalFormat;
        m_DataFormat = dataformat;

        //RC_CORE_ASSERT(textureref == 1, "Failed to load image!");
        if (m_RendererID == 0){
            RC_CORE_ERROR("Couldn't find requested texture file: {0}", m_Path);
            namespace fs = std::filesystem;
            RC_CORE_INFO("System is searching here: {0}", fs::current_path());
        }
        m_Width = width;
        m_Height = height;

        // -- minmap / sampling quality section

        glBindTexture(GL_TEXTURE_2D,m_RendererID);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 1, internalFormat,m_Width,m_Height,0,dataformat,GL_UNSIGNED_BYTE,data);


        GLfloat filter_anx = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY,&filter_anx);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY,filter_anx);

        SOIL_free_image_data(data);
    }
    OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
    : m_Width(width),m_Height(height){
        RC_PROFILE_FUNCTION();
        m_InternalFormat =GL_RGBA8, m_DataFormat =GL_RGBA;

        // -- minmap / sampling quality section
        glGenTextures(1, & m_RendererID);

        glBindTexture(GL_TEXTURE_2D,m_RendererID);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    }
    void OpenGLTexture2D::SetData(void* data, uint32_t size, uint32_t slot) const{
        RC_PROFILE_FUNCTION();
        uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
        RC_CORE_ASSERT(size == m_Width* m_Height * bpp, "Data must be entire texture");

        glBindTexture(GL_TEXTURE_2D,m_RendererID);
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }
    void OpenGLTexture2D::CreateCubeMap(bool with_mipmap) {
        for (int i = 0; i < 6; ++i) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGBA32F,m_Width, m_Height,
                         0, GL_RGBA, GL_FLOAT, nullptr);
        }
        GLenum gl_filter_min = (with_mipmap)? GL_LINEAR_MIPMAP_LINEAR: GL_LINEAR;
        
    }
    void OpenGLTexture2D::Bind(uint32_t slot) const {
        RC_PROFILE_FUNCTION();
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }

    OpenGLTexture2D::~OpenGLTexture2D(){
        RC_PROFILE_FUNCTION();
        glDeleteTextures(1,&m_RendererID);
    }
}

