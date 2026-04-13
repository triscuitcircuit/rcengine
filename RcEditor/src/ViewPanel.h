//
// Created by Tristan Zippert on 10/29/21.
//
#pragma once
#include "RcEngine.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "EditorLayer.h"

#ifndef RCENGINE_VIEWPORT_H
#define RCENGINE_VIEWPORT_H
    namespace RcEngine{
        class ViewPanel : public Layer{
        public:
            ViewPanel(const Ref<EditorLayer>& scene = nullptr);
            virtual ~ViewPanel() = default;

            virtual void OnAttach() override;
            virtual void OnDetach() override;

            void ProcessInput();
            void DrawGizmos();

            void OnUpdate(Timestep ts) override;
            virtual void OnImGuiRender() override;
            void OnEvent(Event& e) override;

            bool isFocused(){return m_isFocused;}

            void SetContext(const Ref<EditorLayer> cxt);
            Ref<EditorLayer>& GetContext(){return m_Context;}
        private:
            bool m_isFocused = true;
            Ref<EditorLayer> m_Context;
        };
}
#endif //RCENGINE_VIEWPORT_H
