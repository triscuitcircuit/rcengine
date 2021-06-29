//
// Created by Tristan Zippert on 6/24/21.
//
#include "rcpch.h"
#include "RcGame2D.h"
#include "external/imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <gtc/type_ptr.hpp>

#include <external/glm/glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

RcGame2D::RcGame2D():
        Layer("RcGameEngine2D"),
        m_CameraController(1280.0f/720.0f, true){

}
void RcGame2D::OnEvent(RcEngine::Event &e) {
    m_CameraController.OnEvent(e);
}
void RcGame2D::OnUpdate(RcEngine::Timestep ts) {

    RC_PROFILE_FUNCTION();

    //Update function
    m_CameraController.OnUpdate(ts);

    // Renderer submission
    RcEngine::RenderCommand::SetClearColor({0.1f,0.1f,0.1f,0.1f});
    RcEngine::RenderCommand::Clear();



    RcEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());

    RcEngine::Renderer2D::DrawQuad({0.0f,0.0f},{10.0f,10.0f},
                                   m_TextureColor,m_BaseTexture, m_TextureTile);

    RcEngine::Renderer2D::DrawQuad({-1.0f,0.0f},{0.8f,0.8f},m_SquareColor);
    //RcEngine::Renderer2D::DrawRotatedQuad({-1.0f,1.0f},{0.8f,0.8f},glm::radians(45.0f),m_SquareColor);


    RcEngine::Renderer2D::EndScene();
}
void RcGame2D::OnImGuiRender() {
    RC_PROFILE_FUNCTION();
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor) );
    ImGui::ColorEdit4("Texture color",glm::value_ptr(m_TextureColor));
    ImGui::SliderFloat("Texture Tiling: ", &m_TextureTile, -5.0f, 20.0f);

    ImGui::Text("X: %f, Y: %f", m_CameraController.GetCamera().GetPosition().x,
                m_CameraController.GetCamera().GetPosition().y);
    ImGui::End();
}
void RcGame2D::OnAttach() {
    RC_PROFILE_FUNCTION();
    m_BaseTexture = RcEngine::Texture2D::Create("Assets/textures/Default.JPG");
    RcEngine::TcpServer();

}
void RcGame2D::OnDetach() {
    RC_PROFILE_FUNCTION();
}