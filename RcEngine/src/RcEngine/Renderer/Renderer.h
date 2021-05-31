//
// Created by Tristan Zippert on 5/27/21.
//
#pragma once
#include "RenderCommand.h"
#include "RenderAPI.h"


namespace RcEngine{
    class Renderer{
    public:
        static void BeginScene();

        static void EndScene();

        static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

        inline static RenderAPI::API GetAPI(){return RenderAPI::GetAPI(); }
    };
}
