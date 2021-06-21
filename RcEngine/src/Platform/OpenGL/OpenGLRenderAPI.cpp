//
// Created by Tristan Zippert on 5/30/21.
//

#include "rcpch.h"
#include "OpenGLRenderAPI.h"

#include <include/glad/glad.h>

namespace RcEngine{
    void OpenGLRenderAPI::SetClearColor(const glm::vec4 color) {
        glClearColor(color.r,color.g,color.b,color.a);
    }
    void OpenGLRenderAPI::Clear() {
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    }
    void OpenGLRenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray) {
        glDrawElements(GL_TRIANGLES,
                       vertexArray->GetIndexBuffer()->GetCount(),GL_UNSIGNED_INT,nullptr);
    }
}