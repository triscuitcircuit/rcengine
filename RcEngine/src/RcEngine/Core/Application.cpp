//
// Created by Tristan Zippert on 5/21/21.
//

//extern "C" void hello();

#include "Application.h"
#include "RcEngine/Log.h"
#include "include/glad/glad.h"

#include "external/GLFW/include/GLFW/glfw3.h"


#include "RcEngine/Input.h"

#define numVAOs 1
GLuint renderingProgram;
GLuint vao[numVAOs];

//struct GLFWwindow;
namespace RcEngine{
#define BIND_EVENT_FN(x) std::bind(&x,this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;
    Application::Application() {

        RC_CORE_ASSERT(!s_Instance,"Application already open");
        s_Instance = this;
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        glGenVertexArrays(1,&m_VertexArray);
        glBindVertexArray(m_VertexArray);

        float vertices[3*3]={
                -0.5f,-0.5f,0.0f,
                0.5f,-0.5f,0.0f,
                0.0f, 0.5f, 0.0f
        };

        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));


        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 3 * sizeof(float), nullptr);



        uint32_t indices[3] = {0,1,2};
        m_IndexBuffer.reset(
                IndexBuffer::Create(indices,sizeof(indices)/sizeof(uint32_t))
                );

        std::string vertShaderStr = R"(
        #version 410

        layout(location =0) in vec3 a_Position;

        out vec4 varyingColor;

        void main(void)
        {
            varyingColor = vec4(a_Position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
            gl_Position = vec4(a_Position,1);
        }
        )";
        std::string fragShaderStr = R"(
            #version 410

            layout(location=0) out vec4 color;
            in vec4 varyingColor;

            void main(void)
            {
                color = varyingColor;

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
 //       hello();
        while (m_Running){
            glClearColor(0.1f,0.1f,0.1f,1);
            glClear(GL_COLOR_BUFFER_BIT);
            m_Shader->Bind();

            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES,m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);


            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);


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