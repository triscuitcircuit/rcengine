//
// Created by Tristan Zippert on 5/30/21.
//

#include "rcpch.h"
#include "OpenGLRenderAPI.h"

#include <include/glad/glad.h>

namespace RcEngine{
    void OpenGLRenderAPI::Init() {
        RC_PROFILE_FUNCTION();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //glEnable(GL_DEPTH_TEST);
    }
    void OpenGLRenderAPI::SetViewport(uint32_t x, uint32_t y,uint32_t width,uint32_t height) {
        glViewport(x,y,width, height);
    }

    void OpenGLRenderAPI::SetClearColor(const glm::vec4 color) {
        glClearColor(color.r,color.g,color.b,color.a);
    }
    void OpenGLRenderAPI::Clear() {
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    }
    void OpenGLRenderAPI::DrawIndexed(const Ref<VertexArray>&  vertexArray, uint32_t indexCount) {
        uint32_t count = indexCount ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
        glDrawElements(GL_TRIANGLES,
                       count,GL_UNSIGNED_INT,nullptr);
        glBindTexture(GL_TEXTURE_2D,0);
    }
}