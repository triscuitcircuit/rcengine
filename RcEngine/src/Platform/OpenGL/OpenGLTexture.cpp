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
        int width,channels,height;
        GLuint  textureRef;

        unsigned char* data = SOIL_load_image(path,&width,&height,&channels,SOIL_LOAD_AUTO);
        textureRef = SOIL_create_OGL_texture(data,&width,&height,channels,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y| SOIL_FLAG_GL_MIPMAPS);

        GLenum internalFormat =0, dataformat =0;
        if(channels == 4){
            internalFormat = GL_RGBA8;
            dataformat = GL_RGBA;
        }else if(channels ==3){
            internalFormat = GL_RGB8;
            dataformat = GL_RGB;
        }

        RC_CORE_ASSERT(textureRef == 1, "Failed to load image!");
        if (textureRef == 0){
            RC_CORE_ERROR("Couldn't find requested texture file: {0}", path);
            namespace fs = std::filesystem;
            RC_CORE_INFO("System is searching here: {0}", fs::current_path());
        }
        m_Width = width;
        m_Height = height;

        // -- minmap / sampling quality section

        glBindTexture(GL_TEXTURE_2D,textureRef);
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


    }
    OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
    : m_Width(width),m_Height(height){

    }
    void OpenGLTexture2D::Bind(uint32_t slot) const {
        glBindTexture(slot, m_RendererID);
    }

    OpenGLTexture2D::~OpenGLTexture2D(){
        glDeleteTextures(1,&m_RendererID);
    }

}

