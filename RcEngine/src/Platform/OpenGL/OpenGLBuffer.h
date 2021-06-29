//
// Created by Tristan Zippert on 5/27/21.
//
#pragma once

#include "RcEngine/Renderer/Buffer.h"

namespace RcEngine{
    class OpenGLVertexBuffer: public VertexBuffer{
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        OpenGLVertexBuffer(uint32_t count);
        virtual ~OpenGLVertexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void SetData(const void* data, uint32_t size) override;

        virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
        virtual const BufferLayout& GetLayout() const override{return m_Layout;}
    private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;
    };

    class OpenGLIndexBuffer: public IndexBuffer{
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        virtual ~OpenGLIndexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual uint32_t GetCount() const override{return m_Count; }
    private:
        uint32_t m_RendererID;
        uint32_t m_Count;
    };

}
