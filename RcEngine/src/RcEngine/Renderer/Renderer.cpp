//
// Created by Tristan Zippert on 5/27/21.
//
#include "rcpch.h"
#include "Renderer.h"
#include "RenderCommand.h"

namespace RcEngine{
    void Renderer::BeginScene() {

    }
    void Renderer::EndScene() {

    }
    void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray) {
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}