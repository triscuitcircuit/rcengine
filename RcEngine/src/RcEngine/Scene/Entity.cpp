//
// Created by Tristan Zippert on 7/7/21.
//

#include "rcpch.h"
#include "Entity.h"

namespace RcEngine{
    Entity::Entity(entt::entity handle, Scene* scene)
        : m_EntityHandle(handle), m_Scene(scene){

    }
}