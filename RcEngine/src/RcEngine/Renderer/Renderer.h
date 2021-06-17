//
// Created by Tristan Zippert on 5/27/21.
//
#pragma once
#include "RenderCommand.h"
#include "RenderAPI.h"
#include "OrthoCamera.h"
#include "Shader.h"


namespace RcEngine{
    class Renderer{
    public:
        static void BeginScene(OrthoCamera& camera);

        static void EndScene();

        static void Submit(const std::shared_ptr<Shader>& shader,
                           const std::shared_ptr<VertexArray>& vertexArray,
                           const glm::mat4& transform = glm::mat4(1.0f));

        inline static RenderAPI::API GetAPI(){return RenderAPI::GetAPI(); }
    private:
        struct SceneData{
            glm::mat4 ViewProjectionMatrix;
        };
        static SceneData* m_SceneData;
    };
}
