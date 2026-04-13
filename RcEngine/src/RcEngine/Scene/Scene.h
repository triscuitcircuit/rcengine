//
// Scene.h - manages entities, 2D/3D rendering, physics, and scripting
//
#pragma once
#include "RcEngine/Core/Timestep.h"
#include "Component.h"
#include "../entt/include/entt.hpp"
#include "RcEngine/Renderer/EditorCamera.h"

#ifndef RCENGINE_CLION_SCENE_H
#define RCENGINE_CLION_SCENE_H

// Forward declarations – keep Jolt and Box2D headers out of this header
class b2World;

namespace RcEngine{
    class Entity;

    class Scene{
    public:
        Scene();
        ~Scene();

        static Ref<Scene> Copy(Ref<Scene> other);

        Entity CreateEntity(const std::string& name = std::string());
        void   DestroyEntity(Entity entity);
        Entity DuplicateEntity(Entity& other);

        // Runtime lifecycle
        void OnRuntimeStart();
        void OnRuntimeStop();

        // Simulation (physics only, no scripts)
        void OnSimulationStart();
        void OnSimulationStop();

        // Per-frame updates
        void OnUpdateRuntime(Timestep ts);
        void OnUpdateSimulation(Timestep ts, EditorCamera& camera);
        void OnUpdateEditor(Timestep ts, EditorCamera& camera);

        void OnViewportReSize(uint32_t width, uint32_t height);

        Entity GetPrimaryCameraEntity();
        Entity CreateEntityWithUUID(UUID uuid, const std::string& name);
        Entity CloneEntityWithUUID(UUID uuid, Entity& other);

    private:
        template<typename T>
        void OnAdded(Entity entity, T& component);

        // 2D physics (Box2D)
        void OnPhysics2DStart();
        void OnPhysics2DStop();

        // 3D physics (Jolt)
        void OnPhysics3DStart();
        void OnPhysics3DStop();

        void RenderScene(EditorCamera& camera);
        void DrawSceneObjects();  // shared 2D+3D draw-loop, called after BeginScene

    private:
        uint32_t m_ViewportWidth  = 0;
        uint32_t m_ViewportHeight = 0;

        entt::registry m_Registry;

        // Box2D 2D physics world
        b2World* m_world = nullptr;

        // Jolt 3D physics – stored as void* to avoid polluting headers with Jolt includes
        void* m_JoltPhysicsSystem  = nullptr;  // JPH::PhysicsSystem*
        void* m_JoltTempAllocator  = nullptr;  // JPH::TempAllocatorImpl*
        void* m_JoltJobSystem      = nullptr;  // JPH::JobSystemSingleThreaded*
        void* m_JoltBPLayerIface   = nullptr;  // BPLayerInterfaceImpl*
        void* m_JoltObjVsBPFilter  = nullptr;  // ObjectVsBroadPhaseLayerFilterImpl*
        void* m_JoltObjLayerFilter = nullptr;  // ObjectLayerPairFilterImpl*

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;
    };
}
#endif //RCENGINE_CLION_SCENE_H
