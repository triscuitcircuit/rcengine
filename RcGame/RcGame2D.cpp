//
// Created by Tristan Zippert on 6/24/21.
//
#include "rcpch.h"
#include "RcGame2D.h"
#include "external/imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <gtc/type_ptr.hpp>

#include <external/glm/glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

static const char* s_MapTiles = "SSSSSSSSSSSSSSSSSSSS"
                                "SSSDDDDDDDDDDDDDSSSS"
                                "SSSDDDDDDDDDDDDDSSSS"
                                "SSSDDDDDDDDDDDDDSSSS"
                                "SSSDDDDDDDDDDDDDSSSS"
                                "SSSDDDDDDDDDDDDDSSSS"
                                "SSSDDDDDDDDDDDDDSSSS"
                                "SSSSSSSSSSSSSSSSSSSS"
                                ;

RcGame2D::RcGame2D():
        Layer("RcGameEngine2D"),
        m_CameraController(1280.0f/720.0f, true){

}
void RcGame2D::OnAttach() {
    RC_PROFILE_FUNCTION();
    m_BaseTexture = RcEngine::Texture2D::Create("Assets/textures/Default.jpg");
    m_SpriteSheet = RcEngine::Texture2D::Create("Assets/Game/Textures/CC_City_Exterior_B.png");
    m_BackgroundSheet = RcEngine::Texture2D::Create("Assets/Game/Textures/CC_City_Exterior_A2.png");

    m_Sprite = RcEngine::SubTexture2D::CreateFromCoords(m_SpriteSheet,{9.5,5.5},{50,50},
                                                        {3,2});
    m_Background = RcEngine::SubTexture2D::CreateFromCoords(m_BackgroundSheet, {0,2},
                                                            {96,96},{1,1});
    m_MapWidth = 14;
    m_MapHeight = strlen(s_MapTiles)/ m_MapWidth;

    s_TextureMap['S'] = RcEngine::SubTexture2D::CreateFromCoords(m_BackgroundSheet, {1,3},
                                                                 {96,96},{1,1});
    s_TextureMap['D'] = RcEngine::SubTexture2D::CreateFromCoords(m_BackgroundSheet, {0,2},
                                                                 {96,96},{1,1});


    m_Particle.ColorBegin = {};
    m_Particle.ColorEnd = {};
    m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
    m_Particle.LifeTime = 1.0f;
    m_Particle.Velocity = {0.0f,0.0f};
    m_Particle.VelocityVariation = {3.0f,1.0f};
    m_Particle.Position = {0.0f,0.0f};
    m_CameraController.SetZoomlevel(5.0f);

    RcEngine::FrameBufferSpec fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;

    RcEngine::TcpServer();

}
void RcGame2D::OnEvent(RcEngine::Event &e) {
    m_CameraController.OnEvent(e);
}
void RcGame2D::OnUpdate(RcEngine::Timestep ts) {

    RC_PROFILE_FUNCTION();

    //Update function
    m_CameraController.OnUpdate(ts);
    RcEngine::Renderer2D::ResetStats();
    {
        // Renderer submission
        RcEngine::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 0.1f});
        RcEngine::RenderCommand::Clear();
    }

    float rotation = ts*(1000000.0f);

    RcEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());


    RcEngine::Renderer2D::DrawQuad({0.0f,0.0f},{10.0f,10.0f},
                                   m_TextureColor,m_BaseTexture, m_TextureTile);

    RcEngine::Renderer2D::DrawQuad({-1.0f,0.0f,1.0f},{0.5f,0.5f},m_SquareColor);
    RcEngine::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f },
                                          m_SquareColor, glm::radians(rotation), m_BaseTexture, m_TextureTile);


    RcEngine::Renderer2D::EndScene();

//    RcEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
//    for(float y = -5.0f; y < 5.0f; y+= 0.5f){
//        for(float x = -5.0f; x < 5.0f; x+= 0.5f){
//            glm::vec4 color = {(x+5.0f)/10.0f,0.4f,(y+5.0f)/10.0f,0.5f};
//            RcEngine::Renderer2D::DrawQuad({x,y},{0.45f,0.45f},color,1.0f);
//        }
//    }
//    RcEngine::Renderer2D::EndScene();

    if(RcEngine::Input::IsMouseButtonPressed(RC_MOUSE_BUTTON_1)){
        auto [x, y] = RcEngine::Input::GetMousePos();
        auto width = RcEngine::Application::Get().GetWindow().GetWidth();
        auto height = RcEngine::Application::Get().GetWindow().GetHeight();

        auto bounds = m_CameraController.GetBounds();
        auto pos = m_CameraController.GetCamera().GetPosition();
        x = (x/width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
        y = bounds.GetHeight() * 0.5f - (y/height) * bounds.GetHeight();
        m_Particle.Position = {x + pos.x, y+ pos.y};
        for(int i =0; i < 5; i++){
            m_ParticleSystem.Emit(m_Particle);
        }
    }
    RcEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
//    RcEngine::Renderer2D::DrawQuad({-1.0f,2.0f},{1.0f,1.0f},
//                                          {1.0f,1.0f,1.0f,1.0f},
//                                          m_Background);
    for(uint32_t y =0; y< m_MapHeight; ++y){
        for (int x = 0; x < m_MapWidth; ++x) {
            char tileType = s_MapTiles[x+y*m_MapWidth];
                RcEngine::Ref<RcEngine::SubTexture2D> defaultTexture;
                    if(s_TextureMap.find(tileType) != s_TextureMap.end()){
                        defaultTexture = s_TextureMap[tileType];
                    }else{
                        defaultTexture = m_Sprite;
                    }
                    RcEngine::Renderer2D::DrawQuad({x-m_MapWidth/2,y-m_MapHeight/2},{1.0,1.0},
                                                   {1.0f,1.0f,1.0f,1.0f},defaultTexture);
        }
    }
    RcEngine::Renderer2D::DrawQuad({-1.0f,2.0f},{1.0f,1.0f},
                                   {1.0f,1.0f,1.0f,1.0f},
                                   m_Sprite);

    RcEngine::Renderer2D::EndScene();
    m_ParticleSystem.OnUpdate(ts);
    m_ParticleSystem.OnRender(m_CameraController.GetCamera());

}
void RcGame2D::OnImGuiRender() {
    RC_PROFILE_FUNCTION();
    ImGui::Begin("Settings");

    ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor) );
    ImGui::ColorEdit4("Texture color",glm::value_ptr(m_TextureColor));
    ImGui::SliderFloat("Texture Tiling: ", &m_TextureTile, -5.0f, 20.0f);
    ImGui::SliderFloat("Rotation control: ", &m_Rotation, -180.0f, 1000.0f);
    ImGui::Text("X: %f, Y: %f", m_CameraController.GetCamera().GetPosition().x,
                m_CameraController.GetCamera().GetPosition().y);


    ImGui::End();
    ImGui::Begin("Renderer 2D Stats");
    auto stats = RcEngine::Renderer2D::GetStats();
    ImGui::Text("Draw Calls: %d",stats.DrawCalls);
    ImGui::Text("Quads: %d",stats.QuadCount);

    ImGui::Text("Vertices %d",stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d",stats.GetTotalIndexCount());

    ImGui::End();
}
void RcGame2D::OnDetach() {
    RC_PROFILE_FUNCTION();
}