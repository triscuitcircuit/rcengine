//
// Renderer3D implementation - Phong-shaded mesh rendering, OpenGL 4.1
//
#include "rcpch.h"
#include "Renderer3D.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "UniformBuffer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace RcEngine {

    // ------------------------------------------------------------------ //
    //  Unit cube (36 vertices, non-indexed sequential draw)               //
    //  Layout: position (3) | normal (3) | uv (2) = 8 floats/vertex      //
    // ------------------------------------------------------------------ //
    static float s_CubeVertices[] = {
        // Back face  (normal  0, 0,-1)
        -0.5f,-0.5f,-0.5f,  0.f, 0.f,-1.f,  0.f,0.f,
         0.5f, 0.5f,-0.5f,  0.f, 0.f,-1.f,  1.f,1.f,
         0.5f,-0.5f,-0.5f,  0.f, 0.f,-1.f,  1.f,0.f,
         0.5f, 0.5f,-0.5f,  0.f, 0.f,-1.f,  1.f,1.f,
        -0.5f,-0.5f,-0.5f,  0.f, 0.f,-1.f,  0.f,0.f,
        -0.5f, 0.5f,-0.5f,  0.f, 0.f,-1.f,  0.f,1.f,
        // Front face (normal  0, 0,+1)
        -0.5f,-0.5f, 0.5f,  0.f, 0.f, 1.f,  0.f,0.f,
         0.5f,-0.5f, 0.5f,  0.f, 0.f, 1.f,  1.f,0.f,
         0.5f, 0.5f, 0.5f,  0.f, 0.f, 1.f,  1.f,1.f,
         0.5f, 0.5f, 0.5f,  0.f, 0.f, 1.f,  1.f,1.f,
        -0.5f, 0.5f, 0.5f,  0.f, 0.f, 1.f,  0.f,1.f,
        -0.5f,-0.5f, 0.5f,  0.f, 0.f, 1.f,  0.f,0.f,
        // Left face  (normal -1, 0, 0)
        -0.5f, 0.5f, 0.5f, -1.f, 0.f, 0.f,  1.f,0.f,
        -0.5f, 0.5f,-0.5f, -1.f, 0.f, 0.f,  1.f,1.f,
        -0.5f,-0.5f,-0.5f, -1.f, 0.f, 0.f,  0.f,1.f,
        -0.5f,-0.5f,-0.5f, -1.f, 0.f, 0.f,  0.f,1.f,
        -0.5f,-0.5f, 0.5f, -1.f, 0.f, 0.f,  0.f,0.f,
        -0.5f, 0.5f, 0.5f, -1.f, 0.f, 0.f,  1.f,0.f,
        // Right face (normal +1, 0, 0)
         0.5f, 0.5f, 0.5f,  1.f, 0.f, 0.f,  1.f,0.f,
         0.5f,-0.5f,-0.5f,  1.f, 0.f, 0.f,  0.f,1.f,
         0.5f, 0.5f,-0.5f,  1.f, 0.f, 0.f,  1.f,1.f,
         0.5f,-0.5f,-0.5f,  1.f, 0.f, 0.f,  0.f,1.f,
         0.5f, 0.5f, 0.5f,  1.f, 0.f, 0.f,  1.f,0.f,
         0.5f,-0.5f, 0.5f,  1.f, 0.f, 0.f,  0.f,0.f,
        // Bottom face (normal  0,-1, 0)
        -0.5f,-0.5f,-0.5f,  0.f,-1.f, 0.f,  0.f,1.f,
         0.5f,-0.5f,-0.5f,  0.f,-1.f, 0.f,  1.f,1.f,
         0.5f,-0.5f, 0.5f,  0.f,-1.f, 0.f,  1.f,0.f,
         0.5f,-0.5f, 0.5f,  0.f,-1.f, 0.f,  1.f,0.f,
        -0.5f,-0.5f, 0.5f,  0.f,-1.f, 0.f,  0.f,0.f,
        -0.5f,-0.5f,-0.5f,  0.f,-1.f, 0.f,  0.f,1.f,
        // Top face   (normal  0,+1, 0)
        -0.5f, 0.5f,-0.5f,  0.f, 1.f, 0.f,  0.f,1.f,
         0.5f, 0.5f, 0.5f,  0.f, 1.f, 0.f,  1.f,0.f,
         0.5f, 0.5f,-0.5f,  0.f, 1.f, 0.f,  1.f,1.f,
         0.5f, 0.5f, 0.5f,  0.f, 1.f, 0.f,  1.f,0.f,
        -0.5f, 0.5f,-0.5f,  0.f, 1.f, 0.f,  0.f,1.f,
        -0.5f, 0.5f, 0.5f,  0.f, 1.f, 0.f,  0.f,0.f,
    };
    static constexpr uint32_t s_CubeVertexCount = 36;

    // ------------------------------------------------------------------ //
    //  Storage                                                             //
    // ------------------------------------------------------------------ //
    struct Renderer3DStorage {
        Ref<Shader>      MeshShader;
        Ref<VertexArray> CubeVAO;

        glm::mat4 ViewProjection = glm::mat4(1.0f);
        glm::vec3 ViewPos        = glm::vec3(0.0f);

        // Global directional light
        glm::vec3 LightDir   = glm::normalize(glm::vec3(0.5f, -1.0f, 0.3f));
        glm::vec3 LightColor = glm::vec3(1.0f);

        Renderer3D::Stats Stats;
    };

    static Renderer3DStorage s_Data;

    // ------------------------------------------------------------------ //
    //  Init / Shutdown                                                     //
    // ------------------------------------------------------------------ //
    void Renderer3D::Init() {
        RC_PROFILE_FUNCTION();

        // Build cube VAO with sequential index buffer
        s_Data.CubeVAO = VertexArray::Create();
        auto vb = VertexBuffer::Create(s_CubeVertices,
                                       s_CubeVertexCount * 8 * sizeof(float));
        vb->SetLayout({
            { ShaderDataType::Float3, "a_Position"  },
            { ShaderDataType::Float3, "a_Normal"    },
            { ShaderDataType::Float2, "a_TexCoords" }
        });
        s_Data.CubeVAO->AddVertexBuffer(vb);

        uint32_t cubeIdx[s_CubeVertexCount];
        for (uint32_t i = 0; i < s_CubeVertexCount; i++) cubeIdx[i] = i;
        s_Data.CubeVAO->SetIndexBuffer(
            IndexBuffer::Create(cubeIdx, s_CubeVertexCount));

        s_Data.MeshShader = Shader::Create("Assets/Shaders/Mesh3D.glsl");
    }

    void Renderer3D::Shutdown() {
        s_Data.MeshShader.reset();
        s_Data.CubeVAO.reset();
    }

    // ------------------------------------------------------------------ //
    //  BeginScene – binds shader and uploads scene-wide uniforms once     //
    // ------------------------------------------------------------------ //
    static void ApplySceneUniforms() {
        auto& sh = s_Data.MeshShader;
        sh->Bind();
        sh->SetMat4  ("u_ViewProjection", s_Data.ViewProjection);
        sh->SetFloat3("u_LightDir",       s_Data.LightDir);
        sh->SetFloat3("u_LightColor",     s_Data.LightColor);
        sh->SetFloat3("u_ViewPos",        s_Data.ViewPos);
    }

    void Renderer3D::BeginScene(const ProjectionCamera& camera) {
        RC_PROFILE_FUNCTION();
        s_Data.ViewProjection = camera.GetViewProjection();
        s_Data.ViewPos        = camera.GetPosition();
        ApplySceneUniforms();
    }

    void Renderer3D::BeginScene(const EditorCamera& camera) {
        RC_PROFILE_FUNCTION();
        s_Data.ViewProjection = camera.GetViewProjection();
        s_Data.ViewPos        = camera.GetPosition();
        ApplySceneUniforms();
    }

    void Renderer3D::BeginScene(const Camera& camera, const glm::mat4& transform) {
        RC_PROFILE_FUNCTION();
        s_Data.ViewProjection = camera.GetProjection() * glm::inverse(transform);
        s_Data.ViewPos        = glm::vec3(transform[3]);
        ApplySceneUniforms();
    }

    void Renderer3D::EndScene() {}

    // ------------------------------------------------------------------ //
    //  Internal: shared DrawMesh implementation                           //
    // ------------------------------------------------------------------ //
    static void DrawMeshInternal(const glm::mat4& transform, Ref<Mesh> mesh,
                                 const glm::vec4& color, Ref<Texture2D> texture,
                                 int entityID) {
        // Normal matrix: transpose(inverse(mat3(model))) – computed on CPU once per draw
        glm::mat4 normalMatrix = glm::mat4(glm::transpose(glm::inverse(glm::mat3(transform))));

        s_Data.MeshShader->SetMat4  ("u_Model",        transform);
        s_Data.MeshShader->SetMat4  ("u_NormalMatrix",  normalMatrix);
        s_Data.MeshShader->SetFloat4("u_Color",         texture ? glm::vec4(1.0f) : color);
        s_Data.MeshShader->SetInt   ("u_UseTexture",    texture ? 1 : 0);
        s_Data.MeshShader->SetInt   ("u_EntityID",      entityID);

        if (texture) {
            s_Data.MeshShader->SetInt("u_Texture", 0);
            texture->Bind(0);
        }

        mesh->Bind();
        RenderCommand::DrawIndexed(mesh->GetVertexArray(), mesh->GetIndexCount());

        s_Data.Stats.DrawCalls++;
        s_Data.Stats.MeshCount++;
    }

    // ------------------------------------------------------------------ //
    //  DrawMesh (flat color)                                               //
    // ------------------------------------------------------------------ //
    void Renderer3D::DrawMesh(const glm::mat4& transform, Ref<Mesh> mesh,
                              const glm::vec4& color, int entityID) {
        if (!mesh || !mesh->IsLoaded()) return;
        DrawMeshInternal(transform, mesh, color, nullptr, entityID);
    }

    // ------------------------------------------------------------------ //
    //  DrawMesh (textured)                                                 //
    // ------------------------------------------------------------------ //
    void Renderer3D::DrawMesh(const glm::mat4& transform, Ref<Mesh> mesh,
                              Ref<Texture2D> texture, int entityID) {
        if (!mesh || !mesh->IsLoaded()) return;
        DrawMeshInternal(transform, mesh, glm::vec4(1.0f), texture, entityID);
    }

    // ------------------------------------------------------------------ //
    //  DrawCube                                                            //
    // ------------------------------------------------------------------ //
    void Renderer3D::DrawCube(const glm::mat4& transform,
                              const glm::vec4& color, int entityID) {
        s_Data.MeshShader->Bind();
        
        glm::mat4 normalMatrix = glm::mat4(glm::transpose(glm::inverse(glm::mat3(transform))));

        s_Data.MeshShader->SetMat4  ("u_Model",        transform);
        s_Data.MeshShader->SetMat4  ("u_NormalMatrix",  normalMatrix);
        s_Data.MeshShader->SetFloat4("u_Color",         color);
        s_Data.MeshShader->SetInt   ("u_UseTexture",    0);
        s_Data.MeshShader->SetInt   ("u_EntityID",      entityID);

        s_Data.CubeVAO->Bind();
        RenderCommand::DrawIndexed(s_Data.CubeVAO, s_CubeVertexCount);

        s_Data.Stats.DrawCalls++;
    }

    // ------------------------------------------------------------------ //
    //  Stats                                                               //
    // ------------------------------------------------------------------ //
    Renderer3D::Stats Renderer3D::GetStats() { return s_Data.Stats; }
    void Renderer3D::ResetStats() {
        memset(&s_Data.Stats, 0, sizeof(Renderer3D::Stats));
    }

}
