//
// Component.h - all ECS components for RcEngine
//
#include <glm/glm.hpp>

#include "SceneCamera.h"
#include "RcEngine/Core/UUID.h"

#include "RcEngine/Renderer/Texture.h"
#include "RcEngine/Renderer/Mesh.h"
#include "RcEngine/Sound/SoundBuffer.h"

#include "glm/gtc/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

// Forward declarations for Lua scripting (avoids circular include)
namespace LUtil { class LuaScript; }

#ifndef RCENGINE_CLION_COMPONENT_H
#define RCENGINE_CLION_COMPONENT_H
namespace RcEngine{

    // ---------------------------------------------------------------- //
    //  Core                                                              //
    // ---------------------------------------------------------------- //
    struct IDComponent{
        UUID ID;
        IDComponent() = default;
        IDComponent(const IDComponent&) = default;
    };

    struct TagComponent{
        std::string Tag;
        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag) : Tag(tag) {}
    };

    struct TransformComponent{
        glm::vec3 Translation = {0.0f,0.0f,0.0f};
        glm::vec3 Rotation    = {0.0f,0.0f,0.0f};
        glm::vec3 Scale       = {1.0f,1.0f,1.0f};

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& translation) : Translation(translation) {}

        glm::mat4 GetTransform() const {
            glm::mat4 rot = glm::toMat4(glm::quat(Rotation));
            return glm::translate(glm::mat4(1.0f), Translation)
                 * rot
                 * glm::scale(glm::mat4(1.0f), Scale);
        }
    };

    // ---------------------------------------------------------------- //
    //  Rendering - 2D                                                    //
    // ---------------------------------------------------------------- //
    struct SpriteRendererComponent{
        glm::vec4    Color{1.0f,1.0f,1.0f,1.0f};
        float        TilingFactor = 1.0f;
        Ref<Texture2D> Texture;

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color) : Color(color) {}

        operator const glm::vec4& () const { return Color; }
        operator       glm::vec4& ()       { return Color; }
    };

    struct CircleRenderComponent{
        glm::vec4 Color{1.0f,1.0f,1.0f,1.0f};
        float Fade      = 0.0005f;
        float Thickness = 1.0f;

        CircleRenderComponent() = default;
        CircleRenderComponent(const CircleRenderComponent&) = default;

        operator const glm::vec4& () const { return Color; }
        operator       glm::vec4& ()       { return Color; }
    };

    struct TextComponent{
        std::string TextString;
        float       lineSpacing = 0.0f;
        float       Kerning     = 0.0f;
        float       MaxWidth    = 0.0f;
        glm::vec4   Color{1.0f,1.0f,1.0f,1.0f};

        TextComponent() = default;
    };

    // ---------------------------------------------------------------- //
    //  Rendering - 3D                                                    //
    // ---------------------------------------------------------------- //
    struct MeshComponent{
        std::string    FilePath;
        Ref<Mesh>      MeshData;         // loaded at runtime
        glm::vec4      Color{1.0f,1.0f,1.0f,1.0f};
        Ref<Texture2D> DiffuseTexture;   // optional

        MeshComponent() = default;
        MeshComponent(const MeshComponent&) = default;
        explicit MeshComponent(const std::string& path) : FilePath(path) {}
    };

    struct CubeComponent{
        glm::vec4 Color{1.0f,1.0f,1.0f,1.0f};

        CubeComponent() = default;
        CubeComponent(const CubeComponent&) = default;
    };

    // ---------------------------------------------------------------- //
    //  Camera                                                            //
    // ---------------------------------------------------------------- //
    struct CameraComponent{
        SceneCamera Camera;
        bool Primary         = true;
        bool FixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };

    // ---------------------------------------------------------------- //
    //  Audio                                                             //
    // ---------------------------------------------------------------- //
    struct SoundComponent{
        Ref<SoundBuffer> Sound;
        float Gain   = 1.0f;
        bool  Playing = false;
        bool  Loop    = false;

        SoundComponent() = default;
        SoundComponent(const SoundComponent&) = default;
    };

    // ---------------------------------------------------------------- //
    //  Scripting - Native C++                                            //
    // ---------------------------------------------------------------- //
    class ScriptableEntity;
    struct NativeScriptComponent{
        ScriptableEntity* Instance = nullptr;

        ScriptableEntity*(*InstantiateScript)();
        void(*DestroyScript)(NativeScriptComponent*);

        template<typename T>
        void Bind(){
            InstantiateScript = []()  { return static_cast<ScriptableEntity*>(new T()); };
            DestroyScript     = [](NativeScriptComponent* nsc) {
                delete nsc->Instance;
                nsc->Instance = nullptr;
            };
        }
    };

    // ---------------------------------------------------------------- //
    //  Scripting - Lua                                                   //
    // ---------------------------------------------------------------- //
    struct LuaScriptComponent {
        std::string                        FilePath;
        std::shared_ptr<LUtil::LuaScript>  Script;   // created during OnRuntimeStart

        LuaScriptComponent() = default;
        LuaScriptComponent(const LuaScriptComponent&) = default;
        explicit LuaScriptComponent(const std::string& path) : FilePath(path) {}
    };

    // ---------------------------------------------------------------- //
    //  Physics - 2D (Box2D)                                              //
    // ---------------------------------------------------------------- //
    struct RigidBodyFlatComponent{
        enum class BodyType { Static = 0, Kinematic = 1, Dynamic = 2 };
        BodyType Type          = BodyType::Static;
        bool     FixedRotation = false;
        void*    RuntimeBody   = nullptr;   // b2Body*

        RigidBodyFlatComponent() = default;
        RigidBodyFlatComponent(const RigidBodyFlatComponent&) = default;
    };

    struct BoxFlatComponent{
        glm::vec2 Offset          = {0.0f,0.0f};
        glm::vec2 Size            = {0.5f,0.5f};
        float     Density         = 1.0f;
        float     Friction        = 0.5f;
        float     Bounce          = 0.5f;
        float     BounceThreshold = 0.5f;
        void*     RuntimeFixture  = nullptr;    // b2Fixture*

        BoxFlatComponent() = default;
        BoxFlatComponent(const BoxFlatComponent&) = default;
    };

    // ---------------------------------------------------------------- //
    //  Physics - 3D (Jolt)                                               //
    // ---------------------------------------------------------------- //
    struct RigidBody3DComponent{
        enum class BodyType { Static = 0, Kinematic = 1, Dynamic = 2 };
        BodyType Type          = BodyType::Static;
        float    LinearDamping = 0.05f;
        float    AngularDamping = 0.05f;
        uint32_t RuntimeBodyID = 0;   // Packed JPH::BodyID index+sequence

        RigidBody3DComponent() = default;
        RigidBody3DComponent(const RigidBody3DComponent&) = default;
    };

    struct BoxCollider3DComponent{
        glm::vec3 HalfExtents = {0.5f,0.5f,0.5f};
        float     Density     = 1000.0f;
        float     Friction    = 0.5f;
        float     Restitution = 0.0f;

        BoxCollider3DComponent() = default;
        BoxCollider3DComponent(const BoxCollider3DComponent&) = default;
    };

    struct SphereCollider3DComponent{
        float Radius      = 0.5f;
        float Density     = 1000.0f;
        float Friction    = 0.5f;
        float Restitution = 0.0f;

        SphereCollider3DComponent() = default;
        SphereCollider3DComponent(const SphereCollider3DComponent&) = default;
    };

    // ---------------------------------------------------------------- //
    //  Misc                                                              //
    // ---------------------------------------------------------------- //
    struct SpinComponent{
        glm::vec3 Rotation = {0.0f,0.0f,0.0f};
        float     speed    = 1.0f;

        SpinComponent() = default;
        SpinComponent(const SpinComponent&) = default;
    };

}
#endif //RCENGINE_CLION_COMPONENT_H
