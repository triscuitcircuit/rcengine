//
// Created by Tristan Zippert on 5/30/21.
//
#pragma once

#include "RenderAPI.h"

#ifndef RCENGINE_CLION_RENDERCOMMAND_H
#define RCENGINE_CLION_RENDERCOMMAND_H

namespace RcEngine{
    class RenderCommand{
    public:
        inline static void SetClearColor(const glm::vec4& color){
            s_RenderAPI->SetClearColor(color);
        }
        inline static void Clear(){
            s_RenderAPI->Clear();
        }
        inline static void DrawIndexed(const Ref<VertexArray>& vertexArray){
            s_RenderAPI->DrawIndexed(vertexArray);
        }

    private:
        static RenderAPI* s_RenderAPI;
    };

}

#endif //RCENGINE_CLION_RENDERCOMMAND_H
