//
// Created by Tristan Zippert on 7/9/21.
//
#pragma once
#include "Scene.h"

#ifndef RCENGINE_SCENESERIALIZER_H
#define RCENGINE_SCENESERIALIZER_H


namespace RcEngine{
        class SceneSerializer{
        public:
            SceneSerializer (const Ref<Scene>& scene);

            void Serialize(const std::string& filepath);
            bool SerializeRuntime(const std::string& filepath);

            bool DeSerialize(const std::string& filepath);
            bool DeSerializeRuntime(const std::string& filepath);
        private:
            Ref<Scene> m_Scene;
        };
}

#endif //RCENGINE_SCENESERIALIZER_H
