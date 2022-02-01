//
// Created by Tristan Zippert on 7/7/21.
//
#include <glm/glm.hpp>

#pragma once
#include "SceneCamera.h"
#include "RcEngine/Core/UUID.h"

#include "RcEngine/Renderer/Texture.h"

#include "glm/gtc/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#ifndef RCENGINE_CLION_COMPONENT_H
#define RCENGINE_CLION_COMPONENT_H
namespace RcEngine{
        struct IDComponent{
            UUID ID;
            IDComponent() = default;
            IDComponent(const IDComponent&) = default;
        };
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
        struct TextComponent{
            std::string TextString;
            float lineSpacing;
            float Kerning;
            float MaxWidth;
            glm::vec4 Color{1.0f,1.0f,1.0f,1.0f};

            TextComponent() = default;

        };
    struct SpriteRendererComponent{
            glm::vec4 Color{1.0f,1.0f,1.0f,1.0f};
            float TilingFactor =1.0f;
            Ref<Texture2D> Texture;

            SpriteRendererComponent() = default;
            SpriteRendererComponent(const SpriteRendererComponent&) = default;
            SpriteRendererComponent(const glm::vec4& color)
                :Color(color){}

            operator const glm::vec4& () const {return Color;}
            operator glm::vec4& () {return Color;}

    };

    struct CubeComponent{
        glm::vec4 Color{1.0f,1.0f,1.0f,1.0f};
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
    class ScriptableEntity;
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
    struct RigidBodyFlatComponent{
        enum class BodyType {Static = 0, Kinematic = 1, Dynamic = 2};
        BodyType Type = BodyType::Static;

        bool FixedRotation = false;

        void* RuntimeBody = nullptr;
        RigidBodyFlatComponent() = default;
        RigidBodyFlatComponent(const RigidBodyFlatComponent&) = default;

    };
    struct CircleRenderComponent{
        glm::vec4 Color{1.0f,1.0f,1.0f,1.0f};
        float Fade = 0.0005;
        float Thickness = 1.0f;
        //Ref<Texture2D> Texture;

        CircleRenderComponent() = default;
        CircleRenderComponent(const CircleRenderComponent&) = default;

        operator const glm::vec4& () const {return Color;}
        operator glm::vec4& () {return Color;}

    };
    struct BoxFlatComponent{

        glm::vec2 Offset ={0.0f,0.0f};
        glm::vec2 Size = {0.5f,0.5f};

        float Density = 1.0f;
        float Friction = 0.5f;
        float Bounce = 0.5f, BounceThreshold = 0.5f;

        void* RuntimeFixture = nullptr;
        BoxFlatComponent() = default;
        BoxFlatComponent(const BoxFlatComponent&) = default;
    };
}
#endif //RCENGINE_CLION_COMPONENT_H
