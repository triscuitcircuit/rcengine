//
// Created by Tristan Zippert on 7/7/21.
//
#pragma once

#include "Scene.h"
#include "external/entt/include/entt.hpp"

#ifndef RCENGINE_CLION_ENTITY_H
#define RCENGINE_CLION_ENTITY_H
namespace RcEngine{
    class Entity{
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene);
        Entity(const Entity& other) = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args){
            RC_CORE_ASSERT(!HasComponent<T>(), "Entity already has component");
            T& component = m_Scene->m_Registry.template emplace<T>(m_EntityHandle,std::forward<Args>(args)...);
            m_Scene->OnAdded<T>(*this,component);
            return component;
        }

        template<typename T>
        T& GetComponent(){
            RC_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        template<typename T>
        bool HasComponent(){
            return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
        }
        template<typename T>
        bool RemoveComponent(){
            RC_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");
            m_Scene->m_Registry.remove<T>(m_EntityHandle);
        }

        operator bool() const{return m_EntityHandle != entt::null;}

        operator uint32_t() const {return (uint32_t)m_EntityHandle;}

        operator entt::entity() const{return m_EntityHandle;}

        bool operator==(const Entity& other) const{return m_EntityHandle== other.m_EntityHandle && m_Scene == other.m_Scene;}
        bool operator!=(const Entity& other)const {
            return !(*this == other);
        }
    private:
        entt::entity m_EntityHandle{ entt::null };
        Scene* m_Scene = nullptr; // 12 byte Entity
    };
}
#endif //RCENGINE_CLION_ENTITY_H
