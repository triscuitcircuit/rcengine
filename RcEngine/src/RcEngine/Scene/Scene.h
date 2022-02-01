//
// Created by Tristan Zippert on 7/6/21.
//
#pragma once
#include "RcEngine/Core/Timestep.h"
#include "Component.h"
#include "../entt/include/entt.hpp"

#include "RcEngine/Renderer/EditorCamera.h"


#ifndef RCENGINE_CLION_SCENE_H
#define RCENGINE_CLION_SCENE_H

class b2World;
namespace RcEngine{
    class Entity;
    class Scene{
    public:
        Scene();
        ~Scene();

        static Ref<Scene> Copy(Ref<Scene> other);

        Entity CreateEntity(const std::string& name=std::string());
        void DestroyEntity(Entity entity);
        Entity DuplicateEntity(Entity& other);

        void OnRuntimeStart();
        void OnRuntimeStop();

        void OnUpdateRuntime(Timestep ts);
        void OnUpdateEditor(Timestep ts, EditorCamera& camera);

        void OnViewportReSize(uint32_t width, uint32_t height);

        Entity GetPrimaryCameraEntity();
        Entity CreateEntityWithUUID(UUID uuid, const std::string& name);
        Entity CloneEntityWithUUID(UUID uuid, Entity& other);

    private:
        template<typename T>
        void OnAdded(Entity entity, T& component);
    private:
        uint32_t m_ViewportWidth = 0,m_ViewportHeight = 0;

        entt::registry m_Registry;

        b2World* m_world = nullptr;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;

    };
}
#endif //RCENGINE_CLION_SCENE_H
