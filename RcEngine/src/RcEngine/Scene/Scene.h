//
// Created by Tristan Zippert on 7/6/21.
//
#pragma once
#include "RcEngine/Core/Timestep.h"
#include "external/entt/include/entt.hpp"


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

        void OnUpdate(Timestep ts);

        void OnViewportReSize(uint32_t width, uint32_t height);

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
