//
// Created by Tristan Zippert on 7/6/21.
//
#pragma once
#include "RcEngine/Core/Timestep.h"
#include "external/entt/include/entt.hpp"

#include "RcEngine/Renderer/EditorCamera.h"


#ifndef RCENGINE_CLION_SCENE_H
#define RCENGINE_CLION_SCENE_H
namespace RcEngine{
    class Entity;
    class Scene{
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name=std::string());
        void DestroyEntity(Entity entity);

        void OnUpdateRuntime(Timestep ts);
        void OnUpdateEditor(Timestep ts, EditorCamera& camera);

        void OnViewportReSize(uint32_t width, uint32_t height);

        Entity GetPrimaryCameraEntity();

    private:
        template<typename T>
        void OnAdded(Entity entity, T& component);
    private:
        uint32_t m_ViewportWidth = 0,m_ViewportHeight = 0;

        entt::registry m_Registry;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;

    };
}
#endif //RCENGINE_CLION_SCENE_H
