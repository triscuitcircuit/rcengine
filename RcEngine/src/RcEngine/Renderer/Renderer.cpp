//
// Created by Tristan Zippert on 5/27/21.
//
#include    "rcpch.h"
#include "Renderer.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "Renderer2D.h"

namespace RcEngine{
    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

    void Renderer::Init() {
        RenderCommand::Init();
        Renderer2D::Init();
    }
    void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
        RenderCommand::SetViewport(0,0,width, height);
    }

    void Renderer::BeginScene(OrthoCamera& camera) {
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectMatrix();
    }
    void Renderer::EndScene() {

    }
    void Renderer::Submit(
            const Ref<Shader>& shader,
            const Ref<VertexArray>& vertexArray,
            const glm::mat4& transform) {

        shader->Bind();

        shader->SetMat4("u_ViewProjection",m_SceneData->ViewProjectionMatrix);
        shader->SetMat4("u_Transform", transform);


        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}