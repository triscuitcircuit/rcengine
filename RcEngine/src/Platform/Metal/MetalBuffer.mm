#include "MetalBuffer.h"


namespace RcEngine {

    MetalVertexBuffer::MetalVertexBuffer(float *vertices, uint32_t size) {

    }

    MetalVertexBuffer::MetalVertexBuffer(uint32_t count) {

    }

    MetalVertexBuffer::~MetalVertexBuffer() {

    }

    void MetalVertexBuffer::Bind() const {

    }

    void MetalVertexBuffer::Unbind() const {

    }

    void MetalVertexBuffer::SetData(const void *data, uint32_t size) {

    }

    void MetalVertexBuffer::SetLayout(const BufferLayout &layout) {

    }

    const BufferLayout &MetalVertexBuffer::GetLayout() const {
        return m_Layout;
    }
}
