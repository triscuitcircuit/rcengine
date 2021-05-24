#include "RcEngine.h"

//Main function in entrypoint.h

class ExampleLayer: public RcEngine::Layer{
public:
    ExampleLayer()
    : Layer("Example"){}

    void OnUpdate() override{
        RC_INFO("ExampleLayer::Update");
    }
    void OnEvent(RcEngine::Event& event) override{
        RC_TRACE("{0}",event);
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