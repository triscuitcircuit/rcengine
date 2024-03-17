//
// Created by Tristan Zippert on 7/6/21.
//
#pragma once
#include "RcEngine.h"
#include "Panels/SpikeGraphPanel.h"

#ifndef RCENGINE_CLION_EDITORLAYER_H
#define RCENGINE_CLION_EDITORLAYER_H
namespace RcEngine{
    class EditorLayer : public Layer{
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        void OnUpdate(Timestep ts) override;
        virtual void OnImGuiRender() override;
        void OnEvent(Event& e) override;
    private:
        bool OnKeyPressed(KeyPressedEvent& e );
        bool OnMouseScroll(MouseScrolledEvent& e);

        void NewScene();
        void OpenScene();
        void OpenScene(const std::filesystem::path& path);
        void SaveAs();

        void OnScenePlay();
        void OnSceneStop();
        void OnSceneEdit();

        void OnSceneSimulation();


        //UI specific functions
        void UI_Toolbar();



    private:
        OrthoCameraController m_CameraController;

        Ref<RcEngine::FrameBuffer> m_FrameBuffer;

        glm::vec4  m_SquareColor = {0.8f,0.0f,0.0f,1.0f};
        glm::vec4  m_TextureColor = {0.2f,0.3f,0.8f,1.0f};

        EditorCamera m_EditorCamera;

        bool m_ViewportFocused = false, m_ViewportHovered = false;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        glm::vec2 m_ViewportBounds[2];

        Entity m_SquareEntity;
        Entity m_CameraEntity;
        Entity m_SecondCamera;
        Ref<Scene> m_ActiveScene;
        Ref<Scene> m_EditorScene;
        std::filesystem::path m_EditorScenePath;

        struct ProfileResult{
            const char* Name;
            float time;
        };
        std::vector<ProfileResult> m_Profiler;


        glm::vec2 m_ViewPortSize = {1264,666};

        //panels
        SpikeGraphPanel m_Panel;

        //editor menu
        Ref<Texture2D> m_IconPlay,m_StopPlay, m_IconSimulate, m_IconSimulateStop;

        //Script
        std::string m_ScriptString;

        int m_GizmoType = -1;

        enum class SceneState{
            Edit =0, Play =1, Simulate=2
        };
        SceneState m_SceneState = SceneState::Edit;



    };
}
#endif //RCENGINE_CLION_EDITORLAYER_H
