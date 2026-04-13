//
// Created by Tristan Zippert on 5/27/21.
//
#include "rcpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace RcEngine{
    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
        switch (Renderer::GetAPI()) {
            case RenderAPI::API::None:
                RC_CORE_ASSERT(false, "No Render API")
                return nullptr;
            case RenderAPI::API::OpenGL:
                //TODO: check OpenGL version
                return std::make_shared<OpenGLVertexBuffer>(size);
            case RenderAPI::API::Vulkan:
                RC_CORE_ASSERT(false, "Vulkan not currently supported");
                return nullptr;
        }
        RC_CORE_ASSERT(false, "Renderer not defined");
        return nullptr;
    }
    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size){
        switch (Renderer::GetAPI()) {
            case RenderAPI::API::None:
                RC_CORE_ASSERT(false, "No Render API")
                return nullptr;
            case RenderAPI::API::OpenGL:
                //TODO: check OpenGL version
                return std::make_shared<OpenGLVertexBuffer>(vertices,size);
            case RenderAPI::API::Vulkan:
                RC_CORE_ASSERT(false, "Vulkan not currently supported");
                return nullptr;
        }
        RC_CORE_ASSERT(false, "Renderer not defined");
        return nullptr;
    }
    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count){
        switch (Renderer::GetAPI()) {
            case RenderAPI::API::None:
                RC_CORE_ASSERT(false, "No Render API");
                return nullptr;
            case RenderAPI::API::OpenGL:
                return std::make_shared<OpenGLIndexBuffer>(indices,count);
            case RenderAPI::API::Vulkan:
                RC_CORE_ASSERT(false, "Vulkan not currently supported");
                return nullptr;
        }
        RC_CORE_ASSERT(false, "Renderer not defined");
        return nullptr;
    }
}