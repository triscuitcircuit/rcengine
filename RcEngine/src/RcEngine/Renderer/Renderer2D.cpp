//
// Created by Tristan Zippert on 6/24/21.
//
#include "rcpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"

#include "RenderCommand.h"
#include <external/glm/glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


namespace RcEngine{
    struct Renderer2DStorage{
        Ref<VertexArray> QuadVertexArray;
        Ref<Texture2D> WhiteTexture;
        Ref<Shader> TextureShader;
    };

    static Renderer2DStorage *s_Data;

    void Renderer2D::Init() {

        s_Data = new Renderer2DStorage();

        s_Data->QuadVertexArray = VertexArray::Create();

        float squareVertices[5 * 4] = {
                -0.5f, -0.5f, 0.0f, 0.0f,0.0f,
                0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
                -0.5f,  0.5f, 0.0f,0.0f, 1.0f
        };

        RcEngine::Ref<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" },
                            });
        s_Data->QuadVertexArray ->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        RcEngine::Ref<IndexBuffer> squareIB;
        squareIB.reset(
                IndexBuffer::Create(squareIndices,
                                    sizeof(squareIndices) / sizeof(uint32_t)));
        s_Data->QuadVertexArray ->SetIndexBuffer(squareIB);

        s_Data->WhiteTexture = Texture2D::Create(1,1);
        uint32_t blankData = 0xffffffff;
        s_Data->WhiteTexture->SetData(&blankData, sizeof(uint32_t));
        s_Data->WhiteTexture->Bind();

        //s_Data->TextureShader = Shader::Create("Assets/Shaders/FlatColor.glsl");
        s_Data->TextureShader = Shader::Create("Assets/Shaders/TextureCombined.glsl");
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetInt("u_Texture",0);

    }
    void Renderer2D::BeginScene(const OrthoCamera &camera) {
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->
                SetMat4("u_ViewProjection",camera.GetViewProjectMatrix());


    }
    void Renderer2D::EndScene() {

    }
    void Renderer2D::Shutdown() {
        delete s_Data;
    }
    void Renderer2D::DrawQuad(const glm::vec2 &position,
                              const glm::vec2 &size,
                              const glm::vec4 &color) {

        DrawQuad({position.x,position.y,0.0f},size,color);

    }
    void Renderer2D::DrawQuad(const glm::vec3 &position,
                              const glm::vec2 &size,
                              const glm::vec4 &color) {

        //s_Data->TextureShader->Bind();
        s_Data->TextureShader->
                SetFloat4("u_Color", color);

        glm::mat4 transform  = glm::translate(glm::mat4(1.0f),position)*
                glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});

        s_Data->TextureShader->
                SetMat4("u_Transform",transform);

        s_Data->QuadVertexArray->Bind();
        s_Data->WhiteTexture->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }
    void Renderer2D::DrawQuad(const glm::vec2 &position,
                              const glm::vec2 &size,const glm::vec4& color,
                              const Ref<Texture2D>& texture) {

        DrawQuad({position.x,position.y,0.0f},size,color,texture);

    }
    void Renderer2D::DrawQuad(const glm::vec3 &position,
                              const glm::vec2 &size,const glm::vec4& color,
                              const Ref<Texture2D>& texture) {

        //s_Data->TextureShader->Bind();
        s_Data->TextureShader->
                SetFloat4("u_Color", color);
        texture->Bind();
        glm::mat4 transform  = glm::translate(glm::mat4(1.0f),position)*
                               glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});

        s_Data->TextureShader->
                SetMat4("u_Transform",transform);


        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }
}