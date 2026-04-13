//
// Created by Tristan Zippert on 7/28/21.
//

#include "Renderer3D.h"
#include "UniformBuffer.h"
#include <external/glm/glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


namespace RcEngine{

    float vertexPositions[108] = {
            -1.0f,  1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,  1.0f,
            1.0f,  1.0f, -1.0f, 1.0f,
            -1.0f,  1.0f, 1.0f,  1.0f,
            1.0f, 1.0f,  1.0f, -1.0f,
            1.0f, -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, 1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f, 1.0f,
            1.0f,  1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,  1.0f,
            -1.0f, 1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f, 1.0f,
            1.0f,  1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f, -1.0f
    };


    struct CubeVertex{
        glm::vec3 WorldPosition;
        glm::vec3 LocalPosition;
        glm::vec4 Color;

        //Editor
        int EntityID;
    };


    struct Renderer3DStorage{
        static const uint32_t MaxTextureSlots = 16; // Render Capacity. APPLE M1: 16 fragment shaders

        Ref<VertexArray> CubeVertexArray;
        Ref<VertexBuffer> CubeVertexBuffer;
        Ref<Texture2D> WhiteTexture;
        Ref<Shader> MeshShader;

        std::array<Ref<Texture2D>,MaxTextureSlots> TextureSlots;

        CubeVertex* CubeVertexBufferBase = nullptr;
        CubeVertex* CubeVertexBufferPtr = nullptr;

        struct CameraData{
            glm::mat4 ViewProjection;
        };
        CameraData CameraBuffer;
        Ref<UniformBuffer> CameraUniformBuffer;
    };

    static Renderer3DStorage s_Data;

    void Renderer3D::Init() {
        RC_PROFILE_FUNCTION();

        s_Data.CubeVertexArray = VertexArray::Create();

        s_Data.CubeVertexBuffer = VertexBuffer::Create(vertexPositions,sizeof(vertexPositions));

        s_Data.CubeVertexBuffer->SetLayout({
           { ShaderDataType::Float3, "a_WorldPosition" },
           { ShaderDataType::Float4, "a_LocalPosition" },
           { ShaderDataType::Float2, "a_Color" },
           { ShaderDataType::Int, "a_EntityID" },
        });
        s_Data.CubeVertexArray->AddVertexBuffer(s_Data.CubeVertexBuffer);

        s_Data.CubeVertexBufferBase = new CubeVertex[108];

        s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DStorage::CameraData),0);


//        s_Data.Cube
    }

    void Renderer3D::Shutdown() {

    }

    void Renderer3D::BeginScene(const Camera &camera, const glm::mat4 transform) {
        RC_PROFILE_FUNCTION();

        glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

        s_Data.MeshShader->Bind();
        s_Data.MeshShader->
                SetMat4("u_ViewProjection",viewProj);
        s_Data.MeshShader->
                SetMat4("mv_matrix",
                        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f)));

    }

    void Renderer3D::BeginScene(const EditorCamera &camera) {

    }

    void Renderer3D::BeginScene(const ProjectionCamera &camera) {
        RC_PROFILE_FUNCTION();


        s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
        s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer3DStorage::CameraData));

    }

    void Renderer3D::EndScene() {

    }

    void Renderer3D::Flush() {

    }
}
