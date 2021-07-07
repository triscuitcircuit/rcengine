//
// Created by Tristan Zippert on 7/6/21.
//
#include "EditorLayer.h"

#include "external/imgui/imgui.h"

#include <gtc/type_ptr.hpp>

#include <external/glm/glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

namespace RcEngine{

    static const char* s_MapTiles = "SSSSSSSSSSSSSSSSSSSS"
                                    "SSSDDDDDDDDDDDDDSSSS"
                                    "SSSDDDDDDDDDDDDDSSSS"
                                    "SSSDDDDDDDDDDDDDSSSS"
                                    "SSSDDDDDDDDDDDDDSSSS"
                                    "SSSDDDDDDDDDDDDDSSSS"
                                    "SSSDDDDDDDDDDDDDSSSS"
                                    "SSSSSSSSSSSSSSSSSSSS"
    ;

    EditorLayer::EditorLayer():
            Layer("RcGameEngine2D"),
            m_CameraController(1280.0f/720.0f, true){

    }
    void EditorLayer::OnAttach() {
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


        m_CameraController.SetZoomlevel(5.0f);

        RcEngine::FrameBufferSpec fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_FrameBuffer = RcEngine::FrameBuffer::Create(fbSpec);

        RcEngine::TcpServer();

    }
    void EditorLayer::OnEvent(RcEngine::Event &e) {
        m_CameraController.OnEvent(e);
    }
    void EditorLayer::OnUpdate(RcEngine::Timestep ts) {

        RC_PROFILE_FUNCTION();

        //Update function
        m_CameraController.OnUpdate(ts);
        RcEngine::Renderer2D::ResetStats();
        {
            // Renderer submission
            m_FrameBuffer->Bind();
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
        m_FrameBuffer->UnBind();

    }
    void EditorLayer::OnImGuiRender() {
        RC_PROFILE_FUNCTION();

        static bool dockingEnabled = true;
        if (dockingEnabled) {
            static bool dockspaceOpen = true;
            static bool opt_fullscreen_persistant = true;
            bool opt_fullscreen = opt_fullscreen_persistant;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
            // because it would be confusing to have two docking targets within each others.
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            if (opt_fullscreen) {
                ImGuiViewport *viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->Pos);
                ImGui::SetNextWindowSize(viewport->Size);
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                                ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }

            // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;

            // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
            // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
            // all active windows docked into it will lose their parent and become undocked.
            // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
            // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
            ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // DockSpace
            ImGuiIO &io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("File")) {
                    // Disabling fullscreen would allow the window to be moved to the front of other windows,
                    // which we can't undo at the moment without finer window depth/z control.
                    //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

                    if (ImGui::MenuItem("Exit")) RcEngine::Application::Get().Close();
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0,0});
            ImGui::Begin("ViewPort");
            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            m_ViewPortSize = {viewportPanelSize.x,viewportPanelSize.y};

            if(m_ViewPortSize != *((glm::vec2*)&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0){
                m_FrameBuffer->Resize((uint32_t)m_ViewPortSize.x,(uint32_t)m_ViewPortSize.y);
                m_ViewPortSize = {viewportPanelSize.x,viewportPanelSize.y};

                m_CameraController.ResizeBounds(viewportPanelSize.x,viewportPanelSize.y);

            }
            uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
            ImGui::Image((void *)textureID, ImVec2{1280, 720},ImVec2{0,1}, ImVec2{1,0});
            ImGui::End();
            ImGui::PopStyleVar();

            ImGui::Begin("Settings");
            ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor) );
            ImGui::ColorEdit4("Texture color",glm::value_ptr(m_TextureColor));
            ImGui::SliderFloat("Texture Tiling: ", &m_TextureTile, -5.0f, 20.0f);
            ImGui::SliderFloat("Rotation control: ", &m_Rotation, -180.0f, 1000.0f);
            ImGui::Text("X: %f, Y: %f", m_CameraController.GetCamera().GetPosition().x,
                        m_CameraController.GetCamera().GetPosition().y);
            //uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
            //ImGui::Image((void*)textureID,ImVec2{320.0f,180.0f});

            ImGui::End();
            ImGui::Begin("Renderer 2D Stats");
            auto stats = RcEngine::Renderer2D::GetStats();
            ImGui::Text("Draw Calls: %d",stats.DrawCalls);
            ImGui::Text("Quads: %d",stats.QuadCount);

            ImGui::Text("Vertices %d",stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d",stats.GetTotalIndexCount());

            ImGui::End();

            ImGui::End();
        }
    }
    void EditorLayer::OnDetach() {
        RC_PROFILE_FUNCTION();
    }
}