//
// Created by Tristan Zippert on 7/7/21.
//
#include <glm.hpp>

#pragma once
#include "SceneCamera.h"

#include "ScriptableEntity.h"
#include "gtc/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>

#ifndef RCENGINE_CLION_COMPONENT_H
#define RCENGINE_CLION_COMPONENT_H
namespace RcEngine{
        struct MeshComponent{
            float value;
            MeshComponent() =default;
        };
        struct TransformComponent{
            glm::vec3 Translation = {0.0f,0.0f,0.0f};
            glm::vec3 Rotation = {0.0f,0.0f,0.0f};
            glm::vec3 Scale = {1.0f,1.0f,1.0f};

            TransformComponent() = default;

            TransformComponent(const TransformComponent&) = default;

            TransformComponent(const glm::vec3 translation)
                    : Translation(translation){}

            glm::mat4 GetTransform() const{
                glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

                return glm::translate(glm::mat4(1.0f),Translation)
                        * rotation
                        * glm::scale(glm::mat4(1.0f),Scale);
            }

        };
    struct SpriteRendererComponent{
            glm::vec4 Color{1.0f,1.0f,1.0f,1.0f};

            SpriteRendererComponent() = default;
            SpriteRendererComponent(const SpriteRendererComponent&) = default;
            SpriteRendererComponent(const glm::vec4& color)
                :Color(color){}

            operator const glm::vec4& () const {return Color;}
            operator glm::vec4& () {return Color;}

    };
    struct TagComponent{
        std::string Tag;
        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag)
            :Tag(tag){}
    };
    struct CameraComponent{
        SceneCamera Camera;
        bool Primary = true;
        bool FixedAspectRatio = false;


        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

    };
    struct NativeScriptComponent{
        ScriptableEntity* Instance = nullptr;

        ScriptableEntity*(*InstantiateScript)();
        void(*DestroyScript)(NativeScriptComponent*);

        template<typename T>
        void Bind(){
            InstantiateScript = [] (){return static_cast<ScriptableEntity*>(new T()); };
            DestroyScript = [](NativeScriptComponent* nsc){delete nsc->Instance; nsc->Instance = nullptr;};
        }

    };
}
#endif //RCENGINE_CLION_COMPONENT_H
