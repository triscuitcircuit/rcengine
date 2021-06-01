//
// Created by Tristan Zippert on 5/29/21.
//
#pragma once
#include <memory>
#include "RcEngine/Renderer/Buffer.h"

#ifndef RCENGINE_CLION_VERTEXARRAY_H
#define RCENGINE_CLION_VERTEXARRAY_H

namespace RcEngine {
    class VertexArray {
    public:

        virtual ~VertexArray() {}

        virtual void Bind() const = 0;

        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) =0;
        virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)=0;

        virtual const std::vector<std::shared_ptr<VertexBuffer>> & GetVertexBuffers() const =0;
        virtual const std::shared_ptr<IndexBuffer> & GetIndexBuffer() const =0;


        static VertexArray *Create();
    };
}

#endif //RCENGINE_CLION_VERTEXARRAY_H