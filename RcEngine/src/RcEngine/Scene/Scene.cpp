//
// Scene.cpp - ECS, 2D/3D physics (Box2D + Jolt), Lua scripting, rendering
//
#include "rcpch.h"
#include "Scene.h"
#include "ScriptableEntity.h"
#include "Entity.h"

#include "RcEngine/Renderer/Renderer2D.h"
#include "RcEngine/Renderer/Renderer3D.h"
#include "RcEngine/Renderer/Mesh.h"
#include "RcEngine/Lua/LuaScript.h"

// Box2D
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

// Jolt Physics
#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemSingleThreaded.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

JPH_SUPPRESS_WARNINGS

// ================================================================== //
//  Jolt layer helpers (file-scope)                                    //
// ================================================================== //
namespace {

    namespace Layers {
        static constexpr JPH::ObjectLayer NON_MOVING = 0;
        static constexpr JPH::ObjectLayer MOVING     = 1;
        static constexpr JPH::uint        NUM_LAYERS = 2;
    }
    namespace BPLayers {
        static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
        static constexpr JPH::BroadPhaseLayer MOVING(1);
        static constexpr JPH::uint            NUM_LAYERS = 2;
    }

    class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface {
    public:
        BPLayerInterfaceImpl() {
            mMap[Layers::NON_MOVING] = BPLayers::NON_MOVING;
            mMap[Layers::MOVING]     = BPLayers::MOVING;
        }
        JPH::uint            GetNumBroadPhaseLayers() const override { return BPLayers::NUM_LAYERS; }
        JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer l) const override { return mMap[l]; }
    private:
        JPH::BroadPhaseLayer mMap[Layers::NUM_LAYERS];
    };

    class ObjVsBPFilterImpl final : public JPH::ObjectVsBroadPhaseLayerFilter {
    public:
        bool ShouldCollide(JPH::ObjectLayer ol, JPH::BroadPhaseLayer bl) const override {
            switch (ol) {
                case Layers::NON_MOVING: return bl == BPLayers::MOVING;
                case Layers::MOVING:     return true;
                default:                 return false;
            }
        }
    };

    class ObjLayerPairFilterImpl final : public JPH::ObjectLayerPairFilter {
    public:
        bool ShouldCollide(JPH::ObjectLayer a, JPH::ObjectLayer b) const override {
            switch (a) {
                case Layers::NON_MOVING: return b == Layers::MOVING;
                case Layers::MOVING:     return true;
                default:                 return false;
            }
        }
    };

} // anonymous namespace

namespace RcEngine {

    // ================================================================== //
    //  Constructor / Destructor                                           //
    // ================================================================== //
    Scene::Scene()  {}
    Scene::~Scene() {}

    // ================================================================== //
    //  Copy                                                               //
    // ================================================================== //
    template<typename T>
    static void CopyComponent(entt::registry& dst, entt::registry& src,
                              const std::unordered_map<UUID, entt::entity>& map) {
        auto view = src.template view<T>();
        for (auto e : view) {
            UUID uuid      = src.template get<IDComponent>(e).ID;
            entt::entity d = map.at(uuid);
            auto& comp     = src.template get<T>(e);
            dst.template emplace_or_replace<T>(d, comp);
        }
    }

    Ref<Scene> Scene::Copy(Ref<Scene> other) {
        Ref<Scene> s = CreateRef<Scene>();
        s->m_ViewportWidth  = other->m_ViewportWidth;
        s->m_ViewportHeight = other->m_ViewportHeight;

        std::unordered_map<UUID, entt::entity> map;
        auto idView = other->m_Registry.view<IDComponent>();
        for (auto e : idView) {
            UUID uuid       = other->m_Registry.get<IDComponent>(e).ID;
            const auto& tag = other->m_Registry.get<TagComponent>(e).Tag;
            map[uuid]       = (entt::entity)s->CreateEntityWithUUID(uuid, tag);
        }

        CopyComponent<TransformComponent>        (s->m_Registry, other->m_Registry, map);
        CopyComponent<SpriteRendererComponent>   (s->m_Registry, other->m_Registry, map);
        CopyComponent<CameraComponent>           (s->m_Registry, other->m_Registry, map);
        CopyComponent<RigidBodyFlatComponent>    (s->m_Registry, other->m_Registry, map);
        CopyComponent<BoxFlatComponent>          (s->m_Registry, other->m_Registry, map);
        CopyComponent<SpinComponent>             (s->m_Registry, other->m_Registry, map);
        CopyComponent<MeshComponent>             (s->m_Registry, other->m_Registry, map);
        CopyComponent<CubeComponent>             (s->m_Registry, other->m_Registry, map);
        CopyComponent<RigidBody3DComponent>      (s->m_Registry, other->m_Registry, map);
        CopyComponent<BoxCollider3DComponent>    (s->m_Registry, other->m_Registry, map);
        CopyComponent<SphereCollider3DComponent> (s->m_Registry, other->m_Registry, map);
        CopyComponent<LuaScriptComponent>        (s->m_Registry, other->m_Registry, map);
        return s;
    }

    // ================================================================== //
    //  Entity management                                                  //
    // ================================================================== //
    Entity Scene::CreateEntity(const std::string& name) {
        return CreateEntityWithUUID(UUID(), name);
    }

    Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name) {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<TransformComponent>();
        entity.AddComponent<IDComponent>(uuid);
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag   = name.empty() ? "Entity" : name;
        return entity;
    }

    void Scene::DestroyEntity(Entity entity) { m_Registry.destroy(entity); }

    Entity Scene::DuplicateEntity(Entity& other) { return CloneEntityWithUUID(UUID(), other); }

    Entity Scene::CloneEntityWithUUID(UUID uuid, Entity& other) {
        Entity entity = { m_Registry.create(), this };
        if (other.HasComponent<SpriteRendererComponent>())
            entity.AddComponent<SpriteRendererComponent>(other.GetComponent<SpriteRendererComponent>());
        if (other.HasComponent<MeshComponent>())
            entity.AddComponent<MeshComponent>(other.GetComponent<MeshComponent>());
        if (other.HasComponent<CubeComponent>())
            entity.AddComponent<CubeComponent>(other.GetComponent<CubeComponent>());
        return entity;
    }

    // ================================================================== //
    //  Viewport                                                           //
    // ================================================================== //
    void Scene::OnViewportReSize(uint32_t width, uint32_t height) {
        m_ViewportWidth  = width;
        m_ViewportHeight = height;
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view) {
            auto& cam = view.get<CameraComponent>(entity);
            if (!cam.FixedAspectRatio)
                cam.Camera.SetViewPortSize(width, height);
        }
    }

    // ================================================================== //
    //  2D Physics                                                         //
    // ================================================================== //
    void Scene::OnPhysics2DStart() {
        m_world = new b2World({0.0f, -9.8f});
        auto view = m_Registry.view<RigidBodyFlatComponent>();
        for (auto e : view) {
            Entity entity   = {e, this};
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& rb        = entity.GetComponent<RigidBodyFlatComponent>();

            b2BodyDef bodyDef;
            bodyDef.type = (b2BodyType)rb.Type;
            bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
            bodyDef.angle = transform.Rotation.z;

            b2Body* body = m_world->CreateBody(&bodyDef);
            body->SetFixedRotation(rb.FixedRotation);
            rb.RuntimeBody = body;

            if (entity.HasComponent<BoxFlatComponent>()) {
                auto& bc = entity.GetComponent<BoxFlatComponent>();
                b2PolygonShape shape;
                shape.SetAsBox(bc.Size.x * transform.Scale.x,
                               bc.Size.y * transform.Scale.y);
                b2FixtureDef fix;
                fix.shape               = &shape;
                fix.density             = bc.Density;
                fix.friction            = bc.Friction;
                fix.restitution         = bc.Bounce;
                fix.restitutionThreshold= bc.BounceThreshold;
                body->CreateFixture(&fix);
            }
        }
    }

    void Scene::OnPhysics2DStop() {
        delete m_world;
        m_world = nullptr;
    }

    // ================================================================== //
    //  3D Physics (Jolt)                                                  //
    // ================================================================== //
    void Scene::OnPhysics3DStart() {
        if (!JPH::Factory::sInstance)
            JPH::Factory::sInstance = new JPH::Factory();
        JPH::RegisterTypes();

        auto* tempAlloc  = new JPH::TempAllocatorImpl(10u * 1024u * 1024u);
        auto* jobSystem  = new JPH::JobSystemSingleThreaded(JPH::cMaxPhysicsJobs);
        auto* bpIface    = new BPLayerInterfaceImpl();
        auto* ovbpFilter = new ObjVsBPFilterImpl();
        auto* olpFilter  = new ObjLayerPairFilterImpl();

        m_JoltTempAllocator = tempAlloc;
        m_JoltJobSystem     = jobSystem;
        m_JoltBPLayerIface  = bpIface;
        m_JoltObjVsBPFilter = ovbpFilter;
        m_JoltObjLayerFilter= olpFilter;

        auto* physSys = new JPH::PhysicsSystem();
        physSys->Init(2048, 0, 2048, 2048, *bpIface, *ovbpFilter, *olpFilter);
        physSys->SetGravity(JPH::Vec3(0.0f, -9.81f, 0.0f));
        m_JoltPhysicsSystem = physSys;

        auto& bi = physSys->GetBodyInterface();

        auto view = m_Registry.view<RigidBody3DComponent>();
        for (auto e : view) {
            Entity entity   = {e, this};
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& rb        = entity.GetComponent<RigidBody3DComponent>();

            JPH::RefConst<JPH::Shape> shape;
            if (entity.HasComponent<BoxCollider3DComponent>()) {
                auto& bc = entity.GetComponent<BoxCollider3DComponent>();
                JPH::BoxShapeSettings ss(JPH::Vec3(
                    bc.HalfExtents.x * transform.Scale.x,
                    bc.HalfExtents.y * transform.Scale.y,
                    bc.HalfExtents.z * transform.Scale.z));
                ss.mDensity = bc.Density;
                shape = ss.Create().Get();
            } else if (entity.HasComponent<SphereCollider3DComponent>()) {
                auto& sc   = entity.GetComponent<SphereCollider3DComponent>();
                float maxS = glm::max(glm::max(transform.Scale.x, transform.Scale.y), transform.Scale.z);
                JPH::SphereShapeSettings ss(sc.Radius * maxS);
                ss.mDensity = sc.Density;
                shape = ss.Create().Get();
            } else {
                JPH::BoxShapeSettings ss(JPH::Vec3(
                    transform.Scale.x * 0.5f,
                    transform.Scale.y * 0.5f,
                    transform.Scale.z * 0.5f));
                shape = ss.Create().Get();
            }

            JPH::EMotionType motionType;
            JPH::ObjectLayer layer;
            switch (rb.Type) {
                case RigidBody3DComponent::BodyType::Static:
                    motionType = JPH::EMotionType::Static;
                    layer      = Layers::NON_MOVING; break;
                case RigidBody3DComponent::BodyType::Kinematic:
                    motionType = JPH::EMotionType::Kinematic;
                    layer      = Layers::MOVING; break;
                default:
                    motionType = JPH::EMotionType::Dynamic;
                    layer      = Layers::MOVING; break;
            }

            glm::quat glmQ = glm::quat(transform.Rotation);
            JPH::BodyCreationSettings bcs(
                shape,
                JPH::RVec3(transform.Translation.x, transform.Translation.y, transform.Translation.z),
                JPH::Quat(glmQ.x, glmQ.y, glmQ.z, glmQ.w),
                motionType, layer);
            bcs.mLinearDamping  = rb.LinearDamping;
            bcs.mAngularDamping = rb.AngularDamping;

            JPH::Body* body = bi.CreateBody(bcs);
            bi.AddBody(body->GetID(), JPH::EActivation::Activate);
            rb.RuntimeBodyID = body->GetID().GetIndexAndSequenceNumber();
        }

        physSys->OptimizeBroadPhase();
    }

    void Scene::OnPhysics3DStop() {
        if (!m_JoltPhysicsSystem) return;

        auto* physSys = static_cast<JPH::PhysicsSystem*>(m_JoltPhysicsSystem);
        auto& bi      = physSys->GetBodyInterface();

        auto view = m_Registry.view<RigidBody3DComponent>();
        for (auto e : view) {
            Entity entity = {e, this};
            auto& rb = entity.GetComponent<RigidBody3DComponent>();
            if (rb.RuntimeBodyID == 0) continue;
            JPH::BodyID id(rb.RuntimeBodyID);
            bi.RemoveBody(id);
            bi.DestroyBody(id);
            rb.RuntimeBodyID = 0;
        }

        delete physSys;
        delete static_cast<JPH::TempAllocatorImpl*>        (m_JoltTempAllocator);
        delete static_cast<JPH::JobSystemSingleThreaded*>  (m_JoltJobSystem);
        delete static_cast<BPLayerInterfaceImpl*>          (m_JoltBPLayerIface);
        delete static_cast<ObjVsBPFilterImpl*>             (m_JoltObjVsBPFilter);
        delete static_cast<ObjLayerPairFilterImpl*>        (m_JoltObjLayerFilter);

        m_JoltPhysicsSystem = m_JoltTempAllocator = m_JoltJobSystem     = nullptr;
        m_JoltBPLayerIface  = m_JoltObjVsBPFilter = m_JoltObjLayerFilter = nullptr;

        JPH::UnregisterTypes();
        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
    }

    // ================================================================== //
    //  Runtime lifecycle                                                  //
    // ================================================================== //
    void Scene::OnRuntimeStart() {
        // Meshes are loaded on-demand in OnAdded<MeshComponent>; no explicit reload needed here.
        OnPhysics2DStart();
        OnPhysics3DStart();

        // Initialise Lua scripts
        auto view = m_Registry.view<LuaScriptComponent>();
        for (auto e : view) {
            Entity entity = {e, this};
            auto& lsc     = entity.GetComponent<LuaScriptComponent>();
            if (lsc.FilePath.empty()) continue;
            lsc.Script = std::make_shared<LUtil::LuaScript>();
            lsc.Script->Initialize(entity, lsc.FilePath);
            lsc.Script->OnCreate();
        }
    }

    void Scene::OnRuntimeStop() {
        auto view = m_Registry.view<LuaScriptComponent>();
        for (auto e : view) {
            Entity entity = {e, this};
            auto& lsc     = entity.GetComponent<LuaScriptComponent>();
            if (lsc.Script) { lsc.Script->OnDestroy(); lsc.Script.reset(); }
        }
        OnPhysics2DStop();
        OnPhysics3DStop();
    }

    void Scene::OnSimulationStart() { OnPhysics2DStart(); OnPhysics3DStart(); }
    void Scene::OnSimulationStop()  { OnPhysics2DStop();  OnPhysics3DStop();  }

    // ================================================================== //
    //  Per-frame update helpers                                           //
    // ================================================================== //
    static void Step2DPhysics(b2World* world, entt::registry& reg, Scene* scene, float ts) {
        if (!world) return;
        world->Step(ts, 6, 2);
        auto view = reg.view<RcEngine::RigidBodyFlatComponent>();
        for (auto e : view) {
            RcEngine::Entity entity = {e, scene};
            auto& transform = entity.GetComponent<RcEngine::TransformComponent>();
            auto& rb        = entity.GetComponent<RcEngine::RigidBodyFlatComponent>();
            b2Body* body    = (b2Body*)rb.RuntimeBody;
            const auto& pos = body->GetPosition();
            transform.Translation.x = pos.x;
            transform.Translation.y = pos.y;
            transform.Rotation.z    = body->GetAngle();
        }
    }

    static void Step3DPhysics(void* sysPtr, void* allocPtr, void* jobPtr,
                               entt::registry& reg, Scene* scene, float ts) {
        if (!sysPtr) return;
        auto* physSys = static_cast<JPH::PhysicsSystem*>(sysPtr);
        physSys->Update(ts, 1,
            static_cast<JPH::TempAllocatorImpl*>(allocPtr),
            static_cast<JPH::JobSystemSingleThreaded*>(jobPtr));

        auto& bi  = physSys->GetBodyInterface();
        auto view = reg.view<RcEngine::RigidBody3DComponent>();
        for (auto e : view) {
            RcEngine::Entity entity = {e, scene};
            auto& transform = entity.GetComponent<RcEngine::TransformComponent>();
            auto& rb        = entity.GetComponent<RcEngine::RigidBody3DComponent>();
            if (rb.RuntimeBodyID == 0) continue;
            if (rb.Type == RcEngine::RigidBody3DComponent::BodyType::Static) continue;

            JPH::BodyID bodyID(rb.RuntimeBodyID);
            JPH::RVec3  pos = bi.GetCenterOfMassPosition(bodyID);
            JPH::Quat   rot = bi.GetRotation(bodyID);

            transform.Translation = { pos.GetX(), pos.GetY(), pos.GetZ() };
            glm::quat glmQ(rot.GetW(), rot.GetX(), rot.GetY(), rot.GetZ());
            transform.Rotation = glm::eulerAngles(glmQ);
        }
    }

    // ================================================================== //
    //  DrawSceneObjects – shared 2D+3D draw loops (call after BeginScene) //
    // ================================================================== //
    void Scene::DrawSceneObjects() {
        // 2D sprites
        {
            auto group = m_Registry.view<TransformComponent, SpriteRendererComponent>();
            for (auto entity : group) {
                auto& tr     = group.get<TransformComponent>(entity);
                auto& sprite = group.get<SpriteRendererComponent>(entity);
                Renderer2D::DrawSprite(tr.GetTransform(), sprite, (int)entity);
            }
        }
        Renderer2D::EndScene();

        // 3D meshes
        {
            auto group = m_Registry.view<TransformComponent, MeshComponent>();
            for (auto entity : group) {
                auto& tr   = group.get<TransformComponent>(entity);
                auto& mesh = group.get<MeshComponent>(entity);
                if (!mesh.MeshData) continue;
                if (mesh.DiffuseTexture)
                    Renderer3D::DrawMesh(tr.GetTransform(), mesh.MeshData, mesh.DiffuseTexture, (int)entity);
                else
                    Renderer3D::DrawMesh(tr.GetTransform(), mesh.MeshData, mesh.Color, (int)entity);
            }
        }
        {
            auto group = m_Registry.view<TransformComponent, CubeComponent>();
            for (auto entity : group) {
                auto& tr   = group.get<TransformComponent>(entity);
                auto& cube = group.get<CubeComponent>(entity);
                Renderer3D::DrawCube(tr.GetTransform(), cube.Color, (int)entity);
            }
        }
        Renderer3D::EndScene();
    }

    // ================================================================== //
    //  RenderScene                                                        //
    // ================================================================== //
    void Scene::RenderScene(EditorCamera& camera) {
        Renderer2D::BeginScene(camera);
        Renderer3D::BeginScene(camera);
        DrawSceneObjects();
    }

    // ================================================================== //
    //  OnUpdateRuntime                                                    //
    // ================================================================== //
    void Scene::OnUpdateRuntime(Timestep ts) {
        // Native scripts
        m_Registry.view<NativeScriptComponent>().each([=](auto e, auto& nsc) {
            if (!nsc.Instance) {
                nsc.Instance = nsc.InstantiateScript();
                nsc.Instance->m_Entity = {e, this};
                nsc.Instance->OnCreate();
            }
            nsc.Instance->OnUpdate(ts);
        });

        // Lua scripts
        {
            auto view = m_Registry.view<LuaScriptComponent>();
            for (auto e : view) {
                auto& lsc = view.get<LuaScriptComponent>(e);
                if (lsc.Script && lsc.Script->IsValid())
                    lsc.Script->OnUpdate(ts);
            }
        }

        Step2DPhysics(m_world, m_Registry, this, ts);
        Step3DPhysics(m_JoltPhysicsSystem, m_JoltTempAllocator, m_JoltJobSystem,
                      m_Registry, this, ts);

        // Render
        Camera* mainCamera = nullptr;
        glm::mat4 camTransform;
        {
            auto group = m_Registry.view<CameraComponent, TransformComponent>();
            for (auto entity : group) {
                auto [cam, tr] = group.get<CameraComponent, TransformComponent>(entity);
                if (cam.Primary) { mainCamera = &cam.Camera; camTransform = tr.GetTransform(); break; }
            }
        }
        if (!mainCamera) return;

        Renderer2D::BeginScene(*mainCamera, camTransform);
        Renderer3D::BeginScene(*mainCamera, camTransform);
        DrawSceneObjects();
    }

    void Scene::OnUpdateSimulation(Timestep ts, EditorCamera& camera) {
        Step2DPhysics(m_world, m_Registry, this, ts);
        Step3DPhysics(m_JoltPhysicsSystem, m_JoltTempAllocator, m_JoltJobSystem,
                      m_Registry, this, ts);
        RenderScene(camera);
    }

    void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera) {
        RenderScene(camera);
    }

    // ================================================================== //
    //  Primary camera                                                     //
    // ================================================================== //
    Entity Scene::GetPrimaryCameraEntity() {
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view) {
            if (view.get<CameraComponent>(entity).Primary)
                return Entity{entity, this};
        }
        return {};
    }

    // ================================================================== //
    //  OnAdded specialisations                                            //
    // ================================================================== //
    template<typename T>
    void Scene::OnAdded(Entity, T&) {
        RC_CORE_ASSERT(false, "Unhandled component in OnAdded");
    }
    template<> void Scene::OnAdded<IDComponent>                (Entity,IDComponent&)                {}
    template<> void Scene::OnAdded<TagComponent>               (Entity,TagComponent&)                {}
    template<> void Scene::OnAdded<TransformComponent>         (Entity,TransformComponent&)          {}
    template<> void Scene::OnAdded<SpriteRendererComponent>    (Entity,SpriteRendererComponent&)     {}
    template<> void Scene::OnAdded<CircleRenderComponent>      (Entity,CircleRenderComponent&)       {}
    template<>
    void Scene::OnAdded<MeshComponent>(Entity, MeshComponent& comp) {
        if (!comp.FilePath.empty() && !comp.MeshData)
            comp.MeshData = Mesh::Create(comp.FilePath);
    }
    template<> void Scene::OnAdded<CubeComponent>              (Entity,CubeComponent&)               {}
    template<> void Scene::OnAdded<SpinComponent>              (Entity,SpinComponent&)               {}
    template<> void Scene::OnAdded<SoundComponent>             (Entity,SoundComponent&)              {}
    template<> void Scene::OnAdded<NativeScriptComponent>      (Entity,NativeScriptComponent&)       {}
    template<> void Scene::OnAdded<LuaScriptComponent>         (Entity,LuaScriptComponent&)          {}
    template<> void Scene::OnAdded<BoxFlatComponent>           (Entity,BoxFlatComponent&)            {}
    template<> void Scene::OnAdded<RigidBodyFlatComponent>     (Entity,RigidBodyFlatComponent&)      {}
    template<> void Scene::OnAdded<RigidBody3DComponent>       (Entity,RigidBody3DComponent&)        {}
    template<> void Scene::OnAdded<BoxCollider3DComponent>     (Entity,BoxCollider3DComponent&)      {}
    template<> void Scene::OnAdded<SphereCollider3DComponent>  (Entity,SphereCollider3DComponent&)   {}

    template<>
    void Scene::OnAdded<CameraComponent>(Entity entity, CameraComponent& component) {
        if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
            component.Camera.SetViewPortSize(m_ViewportWidth, m_ViewportHeight);
    }

}
