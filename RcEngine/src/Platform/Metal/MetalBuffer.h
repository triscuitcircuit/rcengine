/*
 Created by Tristan Zippert on 7/13/21.

 Header for C++ Metal buffer wrapper
*/
#pragma once
#include "MetalResource.h"
#include "RcEngine/Core/Core.h"
#include "RcEngine/Renderer/Buffer.h"

namespace RcEngine{
    class  MetalVertexBuffer: public VertexBuffer{
    public:
        MetalVertexBuffer(float* vertices, uint32_t size);
        MetalVertexBuffer(uint32_t count);

        ~MetalVertexBuffer() override;

        void Bind() const override;

        void Unbind() const override;

        void SetData(const void *data, uint32_t size) override;

        void SetLayout(const BufferLayout &layout) override;

        const BufferLayout &GetLayout() const override;
    public:

        CPPMetalInternal::Buffer objCObj() const;
    private:
        id m_device;
        BufferLayout m_Layout;

        void *m_contentsPtr;
        id m_VertexBuffer;
        id m_IndexBuffer;
    };

}