#include "RcEngine.h"

//Main function in entrypoint.h

#include <external/glm/glm/vec3.hpp> // glm::vec3
#include <external/glm/glm/vec4.hpp> // glm::vec4
#include <external/glm/glm/mat4x4.hpp> // glm::mat4
#include <gtc/type_ptr.hpp>
#include <external/glm/glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include <gtc/matrix_transform.hpp>
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLUtils.h"

glm::mat4 camera(float Translate, glm::vec2 const & Rotate)
{
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
    glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
    View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
    View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    return Projection * View * Model;
}

class ExampleLayer: public RcEngine::Layer{
public:
    ExampleLayer()
    : Layer("Example"), m_Camera(-1.6f,1.6f,-0.9f,0.9f),
    m_CameraPosition(0.0f), m_Pos(0.0f){

        m_VertexArray.reset(RcEngine::VertexArray::Create());


        float vertices[3*7]={
                -0.5f,-0.5f,0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
                0.5f,-0.5f,0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
                0.0f, 0.5f, 0.0f, 0.0f, 0.7f, 0.2f, 1.0f
        };

        RcEngine::Ref<RcEngine::VertexBuffer> vertexBuffer;

        vertexBuffer.reset(RcEngine::VertexBuffer::Create(vertices, sizeof(vertices)));
        RcEngine::BufferLayout layout = {
                {RcEngine::ShaderDataType::Float3, "a_Position"},
                {RcEngine::ShaderDataType::Float4, "a_Color"},
               //{RcEngine::ShaderDataType::Float3, "a_Normal"},
        };
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[3] = {0,1,2};
        RcEngine::Ref<RcEngine::IndexBuffer> indexBuffer;
        indexBuffer.reset(
                RcEngine::IndexBuffer::Create(indices,sizeof(indices)/sizeof(uint32_t))
        );
        m_VertexArray->SetIndexBuffer(indexBuffer);


        m_SquareVA.reset(RcEngine::VertexArray::Create());

        float squareVertices[5 * 4] = {
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
                -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        RcEngine::Ref<RcEngine::VertexBuffer> squareVB;
        squareVB.reset(RcEngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({
                                    { RcEngine::ShaderDataType::Float3, "a_Position" },
                                    { RcEngine::ShaderDataType::Float2, "a_TexCord" }
                            });
        m_SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        RcEngine::Ref<RcEngine::IndexBuffer> squareIB;
        squareIB.reset(RcEngine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);

        std::string vertShaderStr = RcEngine::OpenGLUtils::readShaderFile("Assets/VertShader.glsl");

        std::string fragShaderStr = RcEngine::OpenGLUtils::readShaderFile("Assets/FragShader.glsl");

        m_Shader.reset(RcEngine::Shader::Create(vertShaderStr,fragShaderStr));
        std::string flatColorShaderVertexSrc = RcEngine::OpenGLUtils::readShaderFile("Assets/FlatColorShader.glsl");

        std::string flatColorShaderFragmentSrc = RcEngine::OpenGLUtils::readShaderFile("Assets/FlatColorFrag.glsl");

        m_FlatColorShader.reset(RcEngine::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));

        std::string textureShaderVertexSrc = RcEngine::OpenGLUtils::readShaderFile("Assets/TextureShader.glsl");

        std::string textureShaderFragmentSrc = RcEngine::OpenGLUtils::readShaderFile("Assets/TextureShaderFrag.glsl");


        m_TextureShader.reset(RcEngine::Shader::Create(textureShaderVertexSrc,textureShaderFragmentSrc));

        m_Texture = RcEngine::Texture2D::Create("Assets/textures/Default.JPG");
        std::dynamic_pointer_cast<RcEngine::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture",0);

        auto cam = camera(5.0f,{0.3f,0.3f});
    }

    void OnUpdate(RcEngine::Timestep ts) override{

        //RC_TRACE("Delta time: {0}", ts.GetSeconds());

        if(RcEngine::Input::IsKeyPressed(RC_KEY_A))
            m_CameraPosition.x -= m_CameraSpeed*ts;
        if(RcEngine::Input::IsKeyPressed(RC_KEY_D))
            m_CameraPosition.x += m_CameraSpeed*ts;
        if(RcEngine::Input::IsKeyPressed(RC_KEY_W))
            m_CameraPosition.y -= m_CameraSpeed*ts;
        if(RcEngine::Input::IsKeyPressed(RC_KEY_S))
            m_CameraPosition.y += m_CameraSpeed*ts;

        if(RcEngine::Input::IsKeyPressed(RC_KEY_J))
            m_Pos.x -= m_MoveSpeed*ts;
        if(RcEngine::Input::IsKeyPressed(RC_KEY_L))
            m_Pos.x += m_MoveSpeed*ts;
        if(RcEngine::Input::IsKeyPressed(RC_KEY_K))
            m_Pos.y -= m_MoveSpeed*ts;
        if(RcEngine::Input::IsKeyPressed(RC_KEY_I))
            m_Pos.y += m_MoveSpeed*ts;

        if(RcEngine::Input::IsKeyPressed(RC_KEY_LEFT))
            m_CameraRotation -= m_CameraSpeed*ts;
        if(RcEngine::Input::IsKeyPressed(RC_KEY_RIGHT))
            m_CameraRotation += m_CameraSpeed*ts;
        if(RcEngine::Input::IsKeyPressed(RC_KEY_Y))
           m_CameraPosition *= glm::vec3(m_CameraScaleSpeed);




        RcEngine::RenderCommand::SetClearColor({0.1f,0.1f,0.1f,0.1f});
        RcEngine::RenderCommand::Clear();

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f),m_Pos);


        RcEngine::Renderer::BeginScene(m_Camera);


        std::dynamic_pointer_cast<RcEngine::OpenGLShader>(m_FlatColorShader)->Bind();
        std::dynamic_pointer_cast<RcEngine::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);
        std::dynamic_pointer_cast<RcEngine::OpenGLShader>(m_FlatColorShader)->UploadSampler("u_Sample");


        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        for (int y = 0; y < 20; y++)
        {
            for (int x = 0; x < 20; x++)
            {
                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(4.0f), pos) * scale;
                RcEngine::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
            }
        }

        m_Texture->Bind();
        RcEngine::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
        //Triangle render code
       // RcEngine::Renderer::Submit(m_Shader,m_VertexArray);

        RcEngine::Renderer::EndScene();
    }
    void OnEvent(RcEngine::Event& event) override{
        RcEngine::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<RcEngine::KeyPressedEvent>(
                RC_BIND_EVENT_TO_FUNCTION(ExampleLayer::OnKeyPressedEvent));
    }
    virtual void OnImGuiRender() override{
        ImGui::Begin("Settings");
        ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor) );
        ImGui::Text("X: %f, Y: %f, Z:0",m_CameraPosition.x,m_CameraPosition.y);
        ImGui::End();
    }
    bool OnKeyPressedEvent(RcEngine::KeyPressedEvent& event){
        return false;
    }
private:
    RcEngine::Ref<RcEngine::VertexArray> m_VertexArray;

    RcEngine::Ref<RcEngine::Shader> m_Shader;
    RcEngine::Ref<RcEngine::VertexBuffer> m_VertexBuffer;
    RcEngine::Ref<RcEngine::IndexBuffer> m_IndexBuffer;

    RcEngine::Ref<RcEngine::Texture2D> m_Texture;

    RcEngine::OrthoCamera m_Camera;
    glm::vec3 m_CameraPosition;
    float m_CameraRotation = 100.0f;
    float m_CameraSpeed = 5.0f;
    float m_CameraScaleSpeed = 3.0f;

    RcEngine::Ref<RcEngine::VertexArray> m_SquareVA;

    glm::vec3 m_Pos;
    RcEngine::Ref<RcEngine::Shader> m_FlatColorShader, m_TextureShader;
    glm::vec3 m_SquareColor = {0.2f,0.3f,0.8f};
    float m_MoveSpeed=5.0f;
};

class RcGame:
        public RcEngine::Application{
public:
    RcGame(): RcEngine::Application() {
        PushLayer(new ExampleLayer());
    };
    ~RcGame() { };
};
RcEngine::Application* RcEngine::CreateApplication(){
    return new RcGame();
}