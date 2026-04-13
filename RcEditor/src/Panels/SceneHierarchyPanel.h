//
// Created by Tristan Zippert on 7/8/21.
//
#pragma once

#include "RcEngine/Core/Core.h"

#include "RcEngine/Core/Log.h"
#include "RcEngine/Scene/Entity.h"


#ifndef RCENGINE_CLION_SCENEHIERARCHYPANEL_H
#define RCENGINE_CLION_SCENEHIERARCHYPANEL_H
#include "RcEngine/Scene/Scene.h"
namespace RcEngine{
    class SceneHierarchyPanel{
    public:
        SceneHierarchyPanel()= default;
        SceneHierarchyPanel(const Ref<Scene>& scene);

        void SetContext(const Ref<Scene>& scene);

        void OnImGuiRender();

        Entity GetSelectedEntity() const{return m_Selected;};

    private:
        void DrawEntityNode(Entity entity);
        void DrawComponents(Entity entitySelection);
    private:
        Ref<Scene> m_Scene;
        Entity m_Selected;
        friend class Scene;
    };
}
#endif //RCENGINE_CLION_SCENEHIERARCHYPANEL_H
