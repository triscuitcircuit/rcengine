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
        GLuint  textureRef;
        textureRef = SOIL_load_OGL_texture(path,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

        RC_CORE_ASSERT(textureRef == 1, "Failed to load image!");
        if (textureRef == 0){
            std::cout << "couldn't find texture file: " << path << std::endl;
            namespace fs = std::filesystem;
            std::cout << "Current path is " << fs::current_path() << '\n';
        }

        // -- minmap / sampling quality section

        glBindTexture(GL_TEXTURE_2D,textureRef);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);

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

