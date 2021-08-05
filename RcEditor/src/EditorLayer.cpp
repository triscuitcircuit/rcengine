//
// Created by Tristan Zippert on 7/6/21.
//
#include "EditorLayer.h"

#include "external/imgui/imgui.h"
#include "external/imgui/misc/cpp/imgui_stdlib.h"
#include "RcEngine/Scene/SceneSerializer.h"

#include "RcEngine/Utils/PlatformUtils.h"
#include "external/imguizmo/ImGuizmo.h"


#include <gtc/type_ptr.hpp>
#include "RcEngine/Math/Math.h"

#include <external/glm/glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

namespace RcEngine{

    EditorLayer::EditorLayer():
            Layer("RcGameEngine2D"),
            m_CameraController(1280.0f/720.0f, true){

    }
    void EditorLayer::OnAttach() {
        RC_PROFILE_FUNCTION();

        // Entity
        m_ActiveScene = CreateRef<Scene>();


        auto square = m_ActiveScene->CreateEntity("Red Square");
        square.AddComponent<SpriteRendererComponent>(glm::vec4{1.0f,0.0f,0.0f,1.0f});


        m_SquareEntity = square;

        m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
        m_CameraEntity.AddComponent<CameraComponent>();

        m_SecondCamera = m_ActiveScene->CreateEntity("Second Camera");
        auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
        cc.Primary = false;

        class CameraController: public ScriptableEntity{
        public:
            void OnCreate(){
                std::cout << "Entity Created" << std::endl;
            }
            void OnDestroy(){

            }
            void OnUpdate(Timestep ts){
                auto& translation = GetComponent<TransformComponent>().Translation;
                float speed = 5.0f;

                if(Input::IsKeyPressed(Key::A)){
                    translation.x -= speed * ts;
                }
                if(Input::IsKeyPressed(Key::D)){
                    translation.x += speed * ts;
                }
                if(Input::IsKeyPressed(Key::W)){
                    translation.y += speed * ts;
                }
                if(Input::IsKeyPressed(Key::S)){
                    translation.y -= speed * ts;
                }
            }
        };

        m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();


        m_CameraController.SetZoomlevel(5.0f);

        RcEngine::FrameBufferSpec fbSpec;
        fbSpec.Attachments = {FrameBufferTextureFormat::RGBA8,FrameBufferTextureFormat::RED_INT, FrameBufferTextureFormat::Depth};
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_FrameBuffer = RcEngine::FrameBuffer::Create(fbSpec);

        m_Panel.SetContext(m_ActiveScene);

        m_EditorCamera = EditorCamera(30.0f,1.778f, 0.1f, 1000.0f);

        RcEngine::TcpServer();

    }
    void EditorLayer::OnEvent(RcEngine::Event &e) {

        m_CameraController.OnEvent(e);
        m_EditorCamera.OnEvent(e);

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(RC_BIND_EVENT_TO_FUNCTION(EditorLayer::OnKeyPressed));
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent &e) {
        if(e.GetRepeatCount())
            return false;
        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

        switch(e.GetKeyCode()){
            case Key::S:{
                if (control && shift){
                    SaveAs();
                }
                break;
            }
            case Key::N:{
                if (control){
                    NewScene();
                }
                break;
            }
            case Key::O:{
                if (control){
                    OpenScene();
                }
                break;
            }
            //Gizmo
            case Key::Q:
                m_GizmoType = -1;
                break;
            case Key::W:
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
                break;
            case Key::E:
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
                break;
            case Key::R:
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
                break;
        }
    }

    void EditorLayer::OnUpdate(RcEngine::Timestep ts) {
        RC_PROFILE_FUNCTION();

        if(FrameBufferSpec spec = m_FrameBuffer->GetSpecification();
        m_ViewPortSize.x > 0.0f && m_ViewPortSize.y > 0.0f &&
                (spec.Width != m_ViewPortSize.x || spec.Height != m_ViewPortSize.y)){
            m_FrameBuffer->Resize((uint32_t)m_ViewPortSize.x,(uint32_t)m_ViewPortSize.y);
            m_EditorCamera.SetViewportSize(m_ViewPortSize.x,m_ViewPortSize.y);
            m_ActiveScene->OnViewportReSize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
        }

        m_EditorCamera.OnUpdate(ts);

        if(m_ViewportFocused){
            m_CameraController.OnUpdate(ts);
            m_EditorCamera.OnUpdate(ts);
        }

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

        // Update Scene
        m_ActiveScene->OnUpdateEditor(ts,m_EditorCamera);

        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;
        glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
        my = viewportSize.y - my;

        int mouseX = (int)mx;
        int mouseY = (int)my;

        if(mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
        {
            int pixelData = m_FrameBuffer->ReadPixel(1,mouseX,mouseY);
            RC_CORE_WARN("Pixel Data = {0}", pixelData);
        }

        m_FrameBuffer->UnBind();

    }
    void EditorLayer::OnImGuiRender() {
        RC_PROFILE_FUNCTION();

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
                    if(ImGui::MenuItem("New","Ctrl+N")){
                        NewScene();
                    }
                    if(ImGui::MenuItem("Save As...","Ctrl+Shift+S")){
                        SaveAs();
                    }
                    if(ImGui::MenuItem("Open...","Ctrl+O")){
                        OpenScene();
                    }

                    if (ImGui::MenuItem("Exit")) RcEngine::Application::Get().Close();
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0,0});
            ImGui::Begin("ViewPort");
            auto viewportOffset = ImGui::GetCursorPos();// includes tab

            auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
            auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();

            m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
            m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

            m_ViewportFocused = ImGui::IsWindowFocused();
            m_ViewportHovered = ImGui::IsWindowHovered();
            Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            m_ViewPortSize = {viewportPanelSize.x,viewportPanelSize.y};

            if(m_ViewPortSize != *((glm::vec2*)&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0){
                m_FrameBuffer->Resize((uint32_t)m_ViewPortSize.x,(uint32_t)m_ViewPortSize.y);
                m_ViewPortSize = {viewportPanelSize.x,viewportPanelSize.y};

                m_CameraController.ResizeBounds(viewportPanelSize.x,viewportPanelSize.y);

            }
            uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
            ImGui::Image((void *) textureID, ImVec2{1280, 720},ImVec2{0,1}, ImVec2{1,0});

            auto windowSize = ImGui::GetWindowSize();
            ImVec2 minBound = ImGui::GetWindowPos();
            minBound.x += viewportOffset.x;
            minBound.y += viewportOffset.y;

            ImVec2 maxBound = {minBound.x+ windowSize.x, minBound.y + windowSize.y};
            m_ViewportBounds[0] = {minBound.x, minBound.y};
            m_ViewportBounds[1] = {maxBound.x, maxBound.y};

            //ImGuizmo
            Entity selectedentity = m_Panel.GetSelectedEntity();


            if(selectedentity && m_GizmoType != 1){
                ImGuizmo::SetOrthographic(false);
                ImGuizmo::SetDrawlist();
                float windowwidth = (float)ImGui::GetWindowWidth();
                float windowheight = (float)ImGui::GetWindowHeight();
                ImGuizmo::SetRect(ImGui::GetWindowPos().x,ImGui::GetWindowPos().y,
                                  windowwidth, windowheight);

//
//                auto cameraEntity  = m_ActiveScene->GetPrimaryCameraEntity();
//
//                const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
//                const glm::mat4& cameraProj = camera.GetProjection();
//                glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

                //Editor camera
                const glm::mat4& cameraProj = m_EditorCamera.GetProjection();
                glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

                //Entity Transform
                auto& tc = selectedentity.GetComponent<TransformComponent>();
                glm::mat4 transform = tc.GetTransform();


                bool snap = Input::IsKeyPressed(Key::LeftControl);
                float snapValue = 0.5f;
                //snap to 45 degrees on rotation
                if(m_GizmoType == ImGuizmo::OPERATION::ROTATE)
                    snapValue = 45.0f;

                float snapValues[3] = {snapValue,snapValue,snapValue};

                ImGuizmo::Manipulate(glm::value_ptr(cameraView),glm::value_ptr(cameraProj),
                                     (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform)
                                     , nullptr, snap? snapValues: nullptr);


                if(ImGuizmo::IsUsing()){
                    glm::vec3 translation, rotation, scale;

                    Math::DecomposeTransform(transform, translation,rotation,scale);

                    glm::vec3 deltaRotation = rotation - tc.Rotation;

                    tc.Translation = glm::vec3(transform[3]);
                    tc.Scale = translation;
                    tc.Rotation += deltaRotation;
                    tc.Scale = scale;
                }


            }

            ImGui::End();
            ImGui::PopStyleVar();
            m_Panel.OnImGuiRender();
            m_ContentBrowserPanel.OnImGuiRender();

            ImGui::Begin("Settings");

            uint32_t textureID2 = m_FrameBuffer->GetColorAttachmentRendererID();
            ImGui::Image((void*)textureID2,ImVec2{320.0f,180.0f});

            ImGui::End();
            ImGui::Begin("Renderer 2D Stats");
            auto stats = RcEngine::Renderer2D::GetStats();
            ImGui::Text("Draw Calls: %d",stats.DrawCalls);
            ImGui::Text("Quads: %d",stats.QuadCount);

            ImGui::Text("Vertices %d",stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d",stats.GetTotalIndexCount());

            ImGui::End();
            ImGui::Begin("Script console");
            //ImGui::InputTextMultiline("Script",&m_ScriptString,ImVec2(0,0));
            ImGui::End();

            static bool demo = true;
            ImGui::ShowDemoWindow(&demo);

            ImGui::End();

    }
    void EditorLayer::OnDetach() {
        RC_PROFILE_FUNCTION();
    }

    void EditorLayer::OpenScene() {
        std::string file = FileDialogs::OpenFile("");
        RC_CORE_INFO("Path Selected: {0}", file);
        if(!file.empty()){
            m_ActiveScene = CreateRef<Scene>();
            m_ActiveScene->OnViewportReSize((uint32_t)m_ViewPortSize.x,(uint32_t)m_ViewPortSize.y);
            m_Panel.SetContext(m_ActiveScene);

            SceneSerializer serializer(m_ActiveScene);
            serializer.DeSerialize(file);
        }
    }

    void EditorLayer::NewScene() {
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportReSize((uint32_t)m_ViewPortSize.x,(uint32_t)m_ViewPortSize.y);
        m_Panel.SetContext(m_ActiveScene);
    }

    void EditorLayer::SaveAs() {
        std::string file = FileDialogs::SaveFile("");
        RC_CORE_INFO("file selected: {0}", file);
        if(!file.empty()){
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(file);
        }
    }
}