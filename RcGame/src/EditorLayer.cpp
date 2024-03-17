//
// Created by Tristan Zippert on 7/6/21.
//
#include "EditorLayer.h"
#include "RcEngine/Renderer/RenderAPI.h"

#include "../external/imgui/imgui.h"
#include "../external/imgui/misc/cpp/imgui_stdlib.h"
#include "RcEngine/Scene/SceneSerializer.h"


#include "RcEngine/Utils/PlatformUtils.h"
#include "../external/imguizmo/ImGuizmo.h"



#include <glm/gtc/type_ptr.hpp>
#include "RcEngine/Math/Math.h"
#include <cmath>
#include <../external/glm/glm/gtc/matrix_transform.hpp>

#include "Platform/processordetection.h"

namespace RcEngine{

    static std::vector<float> s_FPS(200);
    static std::vector<float> s_time(200);
    static float s_cur_time = 0.0f;

//    extern const std::filesystem::path g_AssetPath;

    EditorLayer::EditorLayer():
            Layer("RcGameEngine2D"),
            m_CameraController(1280.0f/720.0f, true){
                ImGuiIO& io = ImGui::GetIO();
                (void)io;
                io.IniFilename = "Config/imgui.ini";

    }
    void EditorLayer::OnAttach() {

        RC_PROFILE_FUNCTION();



        m_IconPlay = Texture2D::Create("Assets/Icons/Menu/playicon.png");
        m_StopPlay = Texture2D::Create("Assets/Icons/Menu/stopicon.png");
        m_IconSimulate = Texture2D::Create("Assets/Icons/Menu/simplay.png");
        m_IconSimulateStop = Texture2D::Create("Assets/Icons/Menu/simstop.png");

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



        m_CameraController.SetZoomlevel(5.0f);

        RcEngine::FrameBufferSpec fbSpec;
        fbSpec.Attachments = {FrameBufferTextureFormat::RGBA8,FrameBufferTextureFormat::RED_INT, FrameBufferTextureFormat::Depth};
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_FrameBuffer = RcEngine::FrameBuffer::Create(fbSpec);


        m_EditorCamera = EditorCamera(30.0f,1.778f, 0.1f, 1000.0f);

//        RcEngine::TcpServer();

    }
    void EditorLayer::OnEvent(RcEngine::Event &e) {
        EventDispatcher dispatcher(e);
        if(m_ViewportHovered) {
            m_CameraController.OnEvent(e);
            m_EditorCamera.OnEvent(e);
            dispatcher.Dispatch<KeyPressedEvent>
                    (RC_BIND_EVENT_TO_FUNCTION(EditorLayer::OnKeyPressed));
        }
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

            //TODO: Editor Movement
            case Key::Left:
                break;
            case Key::Right:
                break;
            case Key::Up:
                break;
            case Key::Down:
                break;
            case Key::Space:
                if(shift) {
                    if (m_SceneState == SceneState::Edit) {
                        OnScenePlay();
                    } else {
                        OnSceneStop();
                    }
                }

                break;
        }
    }

    void EditorLayer::OnUpdate(RcEngine::Timestep ts) {
        RC_PROFILE_FUNCTION();
        m_Panel.OnUpdate(ts);


        for (int i = 0; i < s_FPS.size() - 1; ++i) {
            s_FPS[i] = s_FPS[i + 1];
        }

        s_FPS[s_FPS.size()-1] = 1/ts;
        s_cur_time = 1/60.0f;
        double t_s = s_FPS[s_FPS.size() - 1];
        double a_s = s_FPS[s_FPS.size()] - t_s;
        double spike_time = a_s / 0.01;

        s_time[s_time.size() - 1] = spike_time;



        if(FrameBufferSpec spec = m_FrameBuffer->GetSpecification();
        m_ViewPortSize.x > 0.0f && m_ViewPortSize.y > 0.0f &&
                (spec.Width != m_ViewPortSize.x || spec.Height != m_ViewPortSize.y)){
            m_FrameBuffer->Resize((uint32_t)m_ViewPortSize.x,(uint32_t)m_ViewPortSize.y);
            m_EditorCamera.SetViewportSize(m_ViewPortSize.x,m_ViewPortSize.y);
            m_ActiveScene->OnViewportReSize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
        }
        // Update Scene



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

            //RC_CORE_WARN("Pixel Data = {0}", pixelData);
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
            bool aboutWindow = false, settingsWindow = false;
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
                    if(ImGui::MenuItem("About...")){
                            aboutWindow = true;
                        }

                     if (ImGui::MenuItem("Exit")) RcEngine::Application::Get().Close();
                    ImGui::EndMenu();
                }
                if(ImGui::BeginMenu("Edit")){
                    if(ImGui::MenuItem("Settings")){
                        settingsWindow = true;
                    }
                    ImGui::EndMenu();
                }


                ImGui::EndMenuBar();
            }

            if(aboutWindow){
                ImGui::OpenPopup("About");
            }
            if(settingsWindow) ImGui::OpenPopup("Settings");





            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0,0});



            ImGui::PopStyleVar();
            m_Panel.OnImGuiRender();


            if(ImGui::TreeNode("FPS Chart")){

                char fps[32];
                sprintf(fps, "%f", s_FPS[s_FPS.size() - 1]);
                ImGui::PlotLines("FPS", s_FPS.data(),
                                 (int) s_FPS.size(), 0, fps, FLT_MAX, FLT_MAX, ImVec2{0, 80});
                ImGui::TreePop();
            }

            ImGui::End();


        // Create a dummy array of contiguous float values to plot
        // Tip: If your float aren't contiguous but part of a structure, you can pass a pointer to your first float and the sizeof() of your structure in the Stride parameter.
        static float values[90] = { 0 };
        static int values_offset = 0;

        static bool demo = true;
        ImGui::ShowDemoWindow(&demo);

        m_Panel.OnImGuiRender();


    }
    void EditorLayer::UI_Toolbar(){
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2(0,2));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing,ImVec2(0,2));
        ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(0,0,0,0));
        auto& colors = ImGui::GetStyle().Colors;
        const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4(buttonHovered.x,buttonHovered.y,buttonHovered.z,0.5f));
        const auto& buttonActive = colors[ImGuiCol_ButtonActive];
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4(buttonActive.x,buttonActive.y,buttonActive.z,0.5f));

        ImGui::Begin("##toolbar", nullptr,
                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar
                     );
        float size = ImGui::GetWindowHeight()-4;
        {

            Ref<Texture2D> icon = m_SceneState == SceneState::Edit ? m_IconPlay : m_StopPlay;
            ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f)- (size* 0.5f));
            if (ImGui::ImageButton((ImTextureID) icon->GetRendererID(), ImVec2(size, size),
                                   ImVec2(0, 0), ImVec2(1, 1), 0)) {
                if(m_SceneState == SceneState::Edit)
                    OnScenePlay();
                else if(m_SceneState == SceneState::Play)
                    OnSceneStop();
            }
        }

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);

        ImGui::End();

    }
    void EditorLayer::OnScenePlay() {
        m_SceneState = SceneState::Play;
        m_ActiveScene = Scene::Copy(m_EditorScene);
        m_ActiveScene->OnRuntimeStart();

    }
    void EditorLayer::OnSceneStop() {
        if(m_SceneState == SceneState::Play)
            m_ActiveScene->OnRuntimeStop();
        if(m_SceneState == SceneState::Simulate)
            m_ActiveScene->OnSimulationStop();
        m_SceneState = SceneState::Edit;
        m_ActiveScene = m_EditorScene;
    }
    void EditorLayer::OnSceneSimulation() {
        m_SceneState = SceneState::Simulate;
        m_ActiveScene = Scene::Copy(m_EditorScene);
        m_ActiveScene->OnSimulationStart();
    }

    void EditorLayer::OnDetach() {
        RC_PROFILE_FUNCTION();
    }

    void EditorLayer::OpenScene() {
        std::string file = FileDialogs::OpenFile("");
        if(!file.empty())
            OpenScene(file);
    }
    void EditorLayer::OpenScene(const std::filesystem::path& path){
        if(m_SceneState != SceneState::Edit)
            OnSceneStop();

        if(path.extension().string() != ".rc"){
            RC_WARN("Could not load {0} - not an RcEngine file", path.filename().string());
            return;
        }
        Ref<Scene> newScene = CreateRef<Scene>();
        SceneSerializer serializer(newScene);
        if(serializer.DeSerialize(path.string())){
            m_EditorScene = newScene;
            m_EditorScene->OnViewportReSize((uint32_t)m_ViewPortSize.x,(uint32_t)m_ViewPortSize.y);

            m_ActiveScene = m_EditorScene;
            m_EditorScenePath = path;
        }
    }


    void EditorLayer::NewScene() {
        m_EditorScene = CreateRef<Scene>();
        m_EditorScene->OnViewportReSize((uint32_t)m_ViewPortSize.x,(uint32_t)m_ViewPortSize.y);

        m_EditorScenePath = std::filesystem::path();
    }

    void EditorLayer::SaveAs() {
        std::string file = FileDialogs::SaveFile("");
        RC_CORE_INFO("file selected: {0}", file);
        if(!file.empty()){
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(file);
            m_EditorScenePath = file;
        }
    }
}