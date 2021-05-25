#include "RcEngine.h"

//Main function in entrypoint.h

#include <external/glm/glm/vec3.hpp> // glm::vec3
#include <external/glm/glm/vec4.hpp> // glm::vec4
#include <external/glm/glm/mat4x4.hpp> // glm::mat4
#include <external/glm/glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

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
    : Layer("Example"){

        auto cam = camera(5.0f,{0.3f,0.3f});
    }

    void OnUpdate() override{
        //RC_INFO("ExampleLayer::Update");

        if (RcEngine::Input::IsKeyPressed(RC_KEY_TAB)){
            RC_INFO("Tab key was pressed");
        }
    }
    void OnEvent(RcEngine::Event& event) override{
        //RC_TRACE("{0}",event);
    }
};

class RcGame: public RcEngine::Application{
public:
    RcGame() {
        PushLayer(new ExampleLayer());
        PushOverlay(new RcEngine::ImGuiLayer());

    };
    ~RcGame() { };
};
RcEngine::Application* RcEngine::CreateApplication(){
    return new RcGame();
}