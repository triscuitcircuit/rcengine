//
// Created by Tristan Zippert on 5/27/21.
//
#include "rcpch.h"
#include "Renderer.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace RcEngine{
    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

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

        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection",m_SceneData->ViewProjectionMatrix);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);


        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}