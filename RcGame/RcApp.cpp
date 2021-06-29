#include "RcEngine.h"

//Main function in entrypoint.h
#include "RcEngine/Core/EntryPoint.h"

#include <gtc/type_ptr.hpp>
#include <external/glm/glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include <gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

#include "RcGame2D.h"

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
    : Layer("Example"), m_CameraController(1280.0f/720.0f, true),
    m_Pos(0.0f){

        m_VertexArray = RcEngine::VertexArray::Create();


        float vertices[3*7]={
                -0.5f,-0.5f,0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
                0.5f,-0.5f,0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
                0.0f, 0.5f, 0.0f, 0.0f, 0.7f, 0.2f, 1.0f
        };

        RcEngine::Ref<RcEngine::VertexBuffer> vertexBuffer;

        vertexBuffer = RcEngine::VertexBuffer::Create(vertices, sizeof(vertices));
        RcEngine::BufferLayout layout = {
                {RcEngine::ShaderDataType::Float3, "a_Position"},
                {RcEngine::ShaderDataType::Float4, "a_Color"},
        };
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[3] = {0,1,2};
        RcEngine::Ref<RcEngine::IndexBuffer> indexBuffer;
        indexBuffer =
                RcEngine::IndexBuffer::Create(indices,sizeof(indices)/sizeof(uint32_t));
        m_VertexArray->SetIndexBuffer(indexBuffer);


        m_SquareVA = RcEngine::VertexArray::Create();

        float squareVertices[5 * 4] = {
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
                -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        RcEngine::Ref<RcEngine::VertexBuffer> squareVB;
        squareVB = RcEngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
        squareVB->SetLayout({
                                    { RcEngine::ShaderDataType::Float3, "a_Position" },
                                    { RcEngine::ShaderDataType::Float2, "a_TexCord" }
                            });
        m_SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        RcEngine::Ref<RcEngine::IndexBuffer> squareIB;
        squareIB = RcEngine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
        m_SquareVA->SetIndexBuffer(squareIB);

        std::string vertShaderStr = RcEngine::OpenGLUtils::readShaderFile("Assets/Shaders/VertShader.glsl");

        std::string fragShaderStr = RcEngine::OpenGLUtils::readShaderFile("Assets/Shaders/FragShader.glsl");

        m_Shader = RcEngine::Shader::Create("VertTriangle",vertShaderStr,fragShaderStr);
        std::string flatColorShaderVertexSrc = RcEngine::OpenGLUtils::readShaderFile("Assets/Shaders/FlatColorShader.glsl");

        std::string flatColorShaderFragmentSrc = RcEngine::OpenGLUtils::readShaderFile("Assets/Shaders/FlatColorFrag.glsl");

        m_FlatColorShader = RcEngine::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);


        auto textureshader = m_ShaderLibrary.Load( "Assets/Shaders/TextureCombined.glsl");

        m_Texture = RcEngine::Texture2D::Create("Assets/textures/Default.JPG");
        m_TransparentTex = RcEngine::Texture2D::Create("Assets/textures/Texture.png");

        std::dynamic_pointer_cast<RcEngine::OpenGLShader>(textureshader)->UploadUniformInt("u_Texture",0);

        auto cam = camera(5.0f,{0.3f,0.3f});
    }

    void OnUpdate(RcEngine::Timestep ts) override{
        //Update function
        m_CameraController.OnUpdate(ts);

        // Renderer submission
        RcEngine::RenderCommand::SetClearColor({0.1f,0.1f,0.1f,0.1f});
        RcEngine::RenderCommand::Clear();



        glm::mat4 transform = glm::translate(glm::mat4(1.0f),m_Pos);


        RcEngine::Renderer::BeginScene(m_CameraController.GetCamera());


        std::dynamic_pointer_cast<RcEngine::OpenGLShader>(m_FlatColorShader)->Bind();
        std::dynamic_pointer_cast<RcEngine::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);
        std::dynamic_pointer_cast<RcEngine::OpenGLShader>(m_FlatColorShader)->UploadSampler("u_Sample");


        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f));

        for (int y = 0; y < 20; y++)
        {
            for (int x = 0; x < 20; x++)
            {
                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(4.0f), pos) * scale;
                RcEngine::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
            }
        }

        auto textureshader = m_ShaderLibrary.Get("TextureCombined");
        m_Texture->Bind();
        RcEngine::Renderer::Submit(textureshader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));

        //Triangle render code
        RcEngine::Renderer::Submit(m_Shader,m_VertexArray,glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));

        m_TransparentTex->Bind();
        RcEngine::Renderer::Submit(textureshader, m_VertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));


        RcEngine::Renderer::EndScene();
    }
    void OnEvent(RcEngine::Event& event) override{
        m_CameraController.OnEvent(event);
    }
    virtual void OnImGuiRender() override{
    }
    bool OnKeyPressedEvent(RcEngine::KeyPressedEvent& event){
        return false;
    }
private:
    RcEngine::ShaderLibrary m_ShaderLibrary;

    RcEngine::Ref<RcEngine::VertexArray> m_VertexArray;

    RcEngine::Ref<RcEngine::Shader> m_Shader;
    RcEngine::Ref<RcEngine::VertexBuffer> m_VertexBuffer;
    RcEngine::Ref<RcEngine::IndexBuffer> m_IndexBuffer;

    RcEngine::Ref<RcEngine::Texture2D> m_Texture,m_TransparentTex;
    RcEngine::OrthoCameraController m_CameraController;

    RcEngine::Ref<RcEngine::VertexArray> m_SquareVA;

    glm::vec3 m_Pos;
    RcEngine::Ref<RcEngine::Shader> m_FlatColorShader;
    glm::vec3 m_SquareColor = {0.2f,0.3f,0.8f};
};

class RcGame:
        public RcEngine::Application{
public:
    RcGame(): RcEngine::Application() {
        //PushLayer(new ExampleLayer());
        PushLayer(new RcGame2D());
    };
    ~RcGame() { };
};
RcEngine::Application* RcEngine::CreateApplication(){
    return new RcGame();
}