//
// Created by Tristan Zippert on 7/8/21.
//
#pragma once
#include "Entity.h"

#ifndef RCENGINE_CLION_SCRIPTABLEENTITY_H
#define RCENGINE_CLION_SCRIPTABLEENTITY_H
namespace RcEngine{
    class ScriptableEntity{
    public:
        virtual ~ScriptableEntity(){};

        template<typename T>
        T& GetComponent(){

            return m_Entity.template GetComponent<T>();
        }
    protected:
        virtual void OnCreate(){};
        virtual void OnDestroy(){}
        virtual void OnUpdate(Timestep ts){};
    private:
        Entity m_Entity;
        friend class Scene;
    };
}
#endif //RCENGINE_CLION_SCRIPTABLEENTITY_H
