//
// Created by Tristan Zippert on 5/30/21.
//
#pragma once

#include "RcEngine/Renderer/RenderAPI.h"

#ifndef RCENGINE_CLION_OPENGLRENDERAPI_H
#define RCENGINE_CLION_OPENGLRENDERAPI_H

namespace RcEngine{
    class OpenGLRenderAPI: public RenderAPI{
        virtual void Clear() override;
        virtual void SetClearColor(const glm::vec4 color)override;

        virtual void DrawIndexed(const std::shared_ptr<VertexArray>&  vertexArray) override;
    };
}

#endif //RCENGINE_CLION_OPENGLRENDERAPI_H
