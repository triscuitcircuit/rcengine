//
// Created by Tristan Zippert on 5/29/21.
//

#include "OpenGLVertexArray.h"
#include "rcpch.h"
#include <glad/glad.h>


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

        const auto& layout = vertexBuffer->GetLayout();
        RC_CORE_INFO("___VERTEX ATTRIB____");
        for(const auto& element: layout){
            switch (element.Type) {
                case ShaderDataType::Float:
                case ShaderDataType::Float2:
                case ShaderDataType::Float3:
                case ShaderDataType::Float4:
                {
                    glEnableVertexAttribArray(m_VertexBufferIndex);
                    glVertexAttribPointer(m_VertexBufferIndex,element.GetCompCount(),
                                          ShaderDataTypeToGLBase(element.Type)
                            , element.Normalized? GL_TRUE: GL_FALSE
                            , layout.GetStride(), (const void*)element.Offset);


                    RC_CORE_INFO("index {}: comp count:{}, Type:{}, layout:{} ,offset {}",
                                 m_VertexBufferIndex, element.GetCompCount(), ShaderDataTypeToGLBase(element.Type),
                                 layout.GetStride(),  element.Offset);

                    m_VertexBufferIndex++;
                    break;
                }
                case ShaderDataType::Int:
                case ShaderDataType::Int2:
                case ShaderDataType::Int3:
                case ShaderDataType::Int4:
                case ShaderDataType::Bool:
                {
                    glEnableVertexAttribArray(m_VertexBufferIndex);
                    glVertexAttribPointer(m_VertexBufferIndex,
                                          element.GetCompCount(),
                                          ShaderDataTypeToGLBase(element.Type),
                                          element.Normalized? GL_TRUE: GL_FALSE
                            , layout.GetStride()
                            ,(const void*)element.Offset);


                    RC_CORE_INFO("index {}: comp count:{}, Type:{}, layout:{} ,offset {}",
                                 m_VertexBufferIndex, element.GetCompCount(), ShaderDataTypeToGLBase(element.Type),
                                 layout.GetStride(),  element.Offset);

                    m_VertexBufferIndex++;
                    break;

                }
                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4:{
                    uint8_t count = element.GetCompCount();
                    for(uint8_t i=0; i< count ; i++){
                        glEnableVertexAttribArray(m_VertexBufferIndex);
                        glVertexAttribPointer(m_VertexBufferIndex,
                                              count,
                                              ShaderDataTypeToGLBase(element.Type),
                                              element.Normalized? GL_TRUE : GL_FALSE,
                                              layout.GetStride(),
                                              (const void*)(element.Offset + sizeof(float)* count *i));
                        glVertexAttribDivisor(m_VertexBufferIndex,1);
                        m_VertexBufferIndex++;
                    }
                    break;
                }
                default: RC_CORE_ASSERT(false,"Unknown ShaderType");

            }
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
