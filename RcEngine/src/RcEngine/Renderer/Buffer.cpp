//
// Created by Tristan Zippert on 5/27/21.
//
#include "rcpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"



namespace RcEngine{
    VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size){
        switch (Renderer::GetAPI()) {
            case RendererAPI::None:
                RC_CORE_ASSERT(false, "No Render API");
                return nullptr;
            case RendererAPI::OpenGL:
                //TODO: check OpenGL version
                return new OpenGLVertexBuffer(vertices,size);
            case RendererAPI::Vulkan:
                RC_CORE_ASSERT(false, "Vulkan not currently supported");
                return nullptr;
        }
        RC_CORE_ASSERT(false, "Renderer not defined");
        return nullptr;
    }
    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size){
        switch (Renderer::GetAPI()) {
            case RendererAPI::None:
                RC_CORE_ASSERT(false, "No Render API");
                return nullptr;
            case RendererAPI::OpenGL:
                return new OpenGLIndexBuffer(indices,size);
            case RendererAPI::Vulkan:
                RC_CORE_ASSERT(false, "Vulkan not currently supported");
                return nullptr;
        }
        RC_CORE_ASSERT(false, "Renderer not defined");
        return nullptr;
    }
}