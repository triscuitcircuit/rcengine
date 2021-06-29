//
// Created by Tristan Zippert on 5/30/21.
//
#pragma once

#include "RcEngine/Renderer/RenderAPI.h"

#ifndef RCENGINE_CLION_OPENGLRENDERAPI_H
#define RCENGINE_CLION_OPENGLRENDERAPI_H

namespace RcEngine{
    class OpenGLRenderAPI: public RenderAPI{
        virtual void Init() override;

        virtual void SetViewport(uint32_t x, uint32_t y,uint32_t width,uint32_t height) override;

        virtual void Clear() override;
        virtual void SetClearColor(const glm::vec4 color)override;

        virtual void DrawIndexed(const Ref<VertexArray>&  vertexArray, uint32_t indexCount =0)  override;
    };
}

#endif //RCENGINE_CLION_OPENGLRENDERAPI_H
