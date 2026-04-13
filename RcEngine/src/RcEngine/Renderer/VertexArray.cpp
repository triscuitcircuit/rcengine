//
// Created by Tristan Zippert on 5/29/21.
//

#include "rcpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "RenderAPI.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace RcEngine{
     Ref<VertexArray> VertexArray::Create() {
         switch (Renderer::GetAPI()) {
             case RenderAPI::API::None:
                 RC_CORE_ASSERT(false, "No Render API");
                 return nullptr;
             case RenderAPI::API::OpenGL:
                 //TODO: check OpenGL version
                 return std::make_shared<OpenGLVertexArray>();
             case RenderAPI::API::Vulkan:
                 RC_CORE_ASSERT(false, "Vulkan not currently supported");
                 return nullptr;
         }
         RC_CORE_ASSERT(false, "Renderer not defined");
         return nullptr;
     }

}
