//
// Created by Tristan Zippert on 5/21/21.
//

extern "C" void rc_tcp_server();
#include "rcpch.h"
#include "Application.h"
#include "RcEngine/Log.h"
#include "include/glad/glad.h"
#include "RcEngine/Renderer/Renderer.h"

#include "external/GLFW/include/GLFW/glfw3.h"


#include "RcEngine/Input.h"



namespace RcEngine{
#define BIND_EVENT_FN(x) std::bind(&x,this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;
    Application::Application()
    : m_Camera(){
        rc_tcp_server();

        RC_CORE_ASSERT(!s_Instance,"Application already open");
        s_Instance = this;
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        m_VertexArray.reset(VertexArray::Create());


        float vertices[3*7]={
                -0.5f,-0.5f,0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
                0.5f,-0.5f,0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
                0.0f, 0.5f, 0.0f, 0.0f, 0.7f, 0.2f, 1.0f
        };

        std::shared_ptr<VertexBuffer> vertexBuffer;

        vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
        BufferLayout layout = {
                {ShaderDataType::Float3, "a_Position"},
                {ShaderDataType::Float4, "a_Color"},
//               {ShaderDataType::Float3, "a_Normal"},
        };
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);


        uint32_t indices[3] = {0,1,2};
        std::shared_ptr<IndexBuffer> indexBuffer;
        indexBuffer.reset(
                IndexBuffer::Create(indices,sizeof(indices)/sizeof(uint32_t))
                );
        m_VertexArray->SetIndexBuffer(indexBuffer);


        std::string vertShaderStr = R"(
        #version 410 core

        out vec4 color;


        layout(location =0) in vec3 a_Position;
        layout(location =1) in vec4 a_Color;

        uniform mat4 u_ViewProjection;

        out vec3 v_Position;
        out vec4 v_Color;

        void main(void)
        {
            v_Position = a_Position;
            v_Color= vec4(a_Position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);

            v_Color = a_Color;
            gl_Position = u_ViewProjection * vec4(v_Position,1.0);
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

        m_Shader.reset(new Shader(vertShaderStr,fragShaderStr));

    }
    Application::~Application() {

    }
    void Application::PushLayer(Layer *layer) {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }
    void Application::PushOverlay(Layer *layer) {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }
    void Application::OnEvent(Event& e){

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClosed));

        RC_CORE_INFO("{0}",e);

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
            (*--it)->OnEvent(e);
            if(e.m_Handeled)
                break;
        }
    }
    bool Application::OnWindowClosed(WindowCloseEvent &e) {
        m_Running = false;
        return true;
    }



    void Application::Run() {
        while (m_Running){

            RenderCommand::SetClearColor({0.1f,0.1f,0.1f,0.1f});
            RenderCommand::Clear();

            Renderer::BeginScene();

            m_Shader->Bind();
            m_Shader->UploadUniformMat4("u_ViewProjection",m_Camera.m_ProjectionMatrix);
            Renderer::Submit(m_VertexArray);

            Renderer::EndScene();

            for(Layer* layer: m_LayerStack)
                layer->OnUpdate();

            m_ImGuiLayer->Begin();
            for(Layer* layer: m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();
//            std::tuple<float, float> tuple = Input::GetMousePos();
//            RC_CORE_TRACE("{0} {1}", std::get<0>(tuple), std::get<1>(tuple));

            m_Window->OnUpdate();
        }
    }
}