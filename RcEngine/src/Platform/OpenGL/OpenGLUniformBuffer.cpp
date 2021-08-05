//
// Created by Tristan Zippert on 8/2/21.
//
#include "rcpch.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace RcEngine{

    OpenGlUniformBuffer::OpenGlUniformBuffer(uint32_t size, uint32_t binding) {
        glGenBuffers(1, &m_RendererID);
        //testing
        glBufferData(m_RendererID,size,nullptr,GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER,m_RendererID);

    }

    OpenGlUniformBuffer::~OpenGlUniformBuffer() {
        glDeleteBuffers(1,&m_RendererID);
    }

    void OpenGlUniformBuffer::SetData(const void *data, uint32_t size, uint32_t offset) {
        glBufferSubData(m_RendererID,offset,size,data);
    }
}