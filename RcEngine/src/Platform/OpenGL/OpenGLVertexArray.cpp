//
// Created by Tristan Zippert on 5/29/21.
//

#include "OpenGLVertexArray.h"
#include "rcpch.h"

#include <include/glad/glad.h>

namespace RcEngine{
    static GLenum ShaderDataTypeToGLBase(ShaderDataType type){
        switch (type) {
            case ShaderDataType::Float: case ShaderDataType::Float2: case ShaderDataType::Float3:
            case ShaderDataType::Float4: case ShaderDataType::Mat3: case ShaderDataType::Mat4:
                return GL_FLOAT;
            case ShaderDataType::Int: case ShaderDataType::Int2: case ShaderDataType::Int3:
            case ShaderDataType::Int4:
                return GL_INT;
            case ShaderDataType::Bool:
                return GL_BOOL;
        }
        RC_CORE_ASSERT(false, "Unknown ShaderDataType");
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray() {
        RC_PROFILE_FUNCTION();
        glGenVertexArrays(1,&m_RendererID);

    }
    OpenGLVertexArray::~OpenGLVertexArray() {
        RC_PROFILE_FUNCTION();
        glDeleteVertexArrays(1,&m_RendererID);
    }
    void OpenGLVertexArray::Bind() const {
        RC_PROFILE_FUNCTION();
        glBindVertexArray(m_RendererID);
    }
    void OpenGLVertexArray::Unbind() const {
        glBindVertexArray(0);
    }
    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) {
        RC_PROFILE_FUNCTION();
        RC_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(),"Vertex Buffer has no layout");
        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();

        uint32_t index =0;
        const auto& layout = vertexBuffer->GetLayout();
        for(const auto& element: layout){
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,element.GetCompCount(),
                                  ShaderDataTypeToGLBase(element.Type)
                    , element.Normalized? GL_TRUE: GL_FALSE
                    , layout.GetStride(), (const void*)element.Offset);
            index++;
        }
        m_VertexBuffers.push_back(vertexBuffer);

    }
    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer){
        RC_PROFILE_FUNCTION();
        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }
}
