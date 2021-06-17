#include "RcEngine.h"

//Main function in entrypoint.h

#include <external/glm/glm/vec3.hpp> // glm::vec3
#include <external/glm/glm/vec4.hpp> // glm::vec4
#include <external/glm/glm/mat4x4.hpp> // glm::mat4
#include <external/glm/glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include <gtc/matrix_transform.hpp>

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

        std::shared_ptr<RcEngine::VertexBuffer> vertexBuffer;

        vertexBuffer.reset(RcEngine::VertexBuffer::Create(vertices, sizeof(vertices)));
        RcEngine::BufferLayout layout = {
                {RcEngine::ShaderDataType::Float3, "a_Position"},
                {RcEngine::ShaderDataType::Float4, "a_Color"},
               //{RcEngine::ShaderDataType::Float3, "a_Normal"},
        };
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[3] = {0,1,2};
        std::shared_ptr<RcEngine::IndexBuffer> indexBuffer;
        indexBuffer.reset(
                RcEngine::IndexBuffer::Create(indices,sizeof(indices)/sizeof(uint32_t))
        );
        m_VertexArray->SetIndexBuffer(indexBuffer);


        std::string vertShaderStr = R"(
        #version 410 core

        out vec4 color;


        layout(location =0) in vec3 a_Position;
        layout(location =1) in vec4 a_Color;

        uniform mat4 u_ViewProjection;
        uniform mat4 u_Transform;

        out vec3 v_Position;
        out vec4 v_Color;

        void main(void)
        {
            v_Position = a_Position;
            v_Color= vec4(a_Position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);

            v_Color = a_Color;
            gl_Position = u_ViewProjection * u_Transform * vec4(v_Position,1.0);
        }
        )";
        std::string fragShaderStr = R"(
            #version 410 core

            layout(location=0) out vec4 color;

            in vec4 v_Color;

            void main(void)
            {
                color = v_Color;

            }
        )";

        m_Shader.reset(new RcEngine::Shader(vertShaderStr,fragShaderStr));


        auto cam = camera(5.0f,{0.3f,0.3f});
    }

    void OnUpdate(RcEngine::Timestep ts) override{

        RC_TRACE("Delta time: {0}", ts.GetSeconds());

        if(RcEngine::Input::IsKeyPressed(RC_KEY_A))
            m_CameraPosition.x -= m_CameraSpeed*ts;
        if(RcEngine::Input::IsKeyPressed(RC_KEY_D))
            m_CameraPosition.x += m_CameraSpeed*ts;
        if(RcEngine::Input::IsKeyPressed(RC_KEY_W))
            m_CameraPosition.y -= m_CameraSpeed*ts;
        if(RcEngine::Input::IsKeyPressed(RC_KEY_S))
            m_CameraPosition.y += m_CameraSpeed*ts;

        if(RcEngine::Input::IsKeyPressed(RC_KEY_I))
            m_Pos.x -= m_MoveSpeed*ts;
        if(RcEngine::Input::IsKeyPressed(RC_KEY_J))
            m_Pos.x += m_MoveSpeed*ts;
        if(RcEngine::Input::IsKeyPressed(RC_KEY_K))
            m_Pos.y -= m_MoveSpeed*ts;
        if(RcEngine::Input::IsKeyPressed(RC_KEY_L))
            m_Pos.y += m_MoveSpeed*ts;

        if(RcEngine::Input::IsKeyPressed(RC_KEY_LEFT))
            m_CameraRotation -= m_CameraSpeed*ts;
        if(RcEngine::Input::IsKeyPressed(RC_KEY_RIGHT))
            m_CameraRotation += m_CameraSpeed*ts;


        RcEngine::RenderCommand::SetClearColor({0.1f,0.1f,0.1f,0.1f});
        RcEngine::RenderCommand::Clear();

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f),m_Pos);


        RcEngine::Renderer::BeginScene(m_Camera);

        RcEngine::Renderer::Submit(m_Shader,m_VertexArray,transform);

        RcEngine::Renderer::EndScene();
    }
    void OnEvent(RcEngine::Event& event) override{
        RcEngine::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<RcEngine::KeyPressedEvent>(
                RC_BIND_EVENT_TO_FUNCTION(ExampleLayer::OnKeyPressedEvent));
    }
    bool OnKeyPressedEvent(RcEngine::KeyPressedEvent& event){

        return false;
    }
private:
    std::shared_ptr<RcEngine::VertexArray> m_VertexArray;

    std::shared_ptr<RcEngine::Shader> m_Shader;
    std::shared_ptr<RcEngine::VertexBuffer> m_VertexBuffer;
    std::shared_ptr<RcEngine::IndexBuffer> m_IndexBuffer;

    std::shared_ptr<RcEngine::VertexArray> m_VA;

    RcEngine::OrthoCamera m_Camera;
    glm::vec3 m_CameraPosition;
    float m_CameraRotation = 10.0f;
    float m_CameraSpeed = 5.0f;

    glm::vec3 m_Pos;
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