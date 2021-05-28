//
// Created by Tristan Zippert on 5/27/21.
//
#include "OpenGLBuffer.h"
#include "rcpch.h"


#include <include/glad/glad.h>

namespace RcEngine{
    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
    {
        glShadeModel(GL_FLAT);
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER,m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    }
    OpenGLVertexBuffer::~OpenGLVertexBuffer(){
        glDeleteBuffers(1, &m_RendererID);
    }
    void OpenGLVertexBuffer::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER,m_RendererID);
    }
    void OpenGLVertexBuffer::Unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER,0);
    }

    //-----------------------------------------------------------------------------------
    //Index Buffer-----------------------------------------------------------------------
    //-----------------------------------------------------------------------------------

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t *indices, uint32_t count)
    :m_Count(count){
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count* sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }
    OpenGLIndexBuffer::~OpenGLIndexBuffer() {
        glDeleteBuffers(1, &m_RendererID);
    }
    void OpenGLIndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    }
    void OpenGLIndexBuffer::Unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    }
}