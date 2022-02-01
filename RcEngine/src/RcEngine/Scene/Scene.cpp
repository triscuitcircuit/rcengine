//
// Created by Tristan Zippert on 7/6/21.
//
#include "rcpch.h"
#include "Scene.h"
#include "ScriptableEntity.h"

#include "Entity.h"

#include "RcEngine/Renderer/Renderer2D.h"


#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"


namespace RcEngine{
    Scene::Scene() {
    }
    Scene::~Scene() {

    }
    template<typename T>
    static void CopyComponent(entt::registry& dst, entt::registry& src,
                              const std::unordered_map<UUID, entt::entity>& enttmap){
        auto view = src.template view<T>();
        for(auto e: view){
            UUID uuid = src.template get<IDComponent>(e).ID;
            entt::entity dstenttID = enttmap.at(uuid);
            auto& component = src.template get<T>(e);

            dst.template emplace_or_replace<T>(dstenttID, component);
        }
    }
    Ref<Scene> Scene::Copy(Ref<Scene> other){
        Ref<Scene> newScene = CreateRef<Scene>();

        newScene->m_ViewportWidth = other->m_ViewportWidth;
        newScene->m_ViewportHeight = other->m_ViewportHeight;

        std::unordered_map<UUID, entt::entity> enttMap;

        auto& srcSceneReg = other->m_Registry;
        auto& dstSceneReg = newScene->m_Registry;
        auto idView = srcSceneReg.view<IDComponent>();
        for (auto e: idView){
            UUID uuid = srcSceneReg.get<IDComponent>(e).ID;
            const auto& name= srcSceneReg.get<TagComponent>(e).Tag;
            enttMap[uuid] = (entt::entity)newScene->CreateEntityWithUUID(uuid, name);
        }

        CopyComponent<TransformComponent>(dstSceneReg,srcSceneReg,enttMap);
        CopyComponent<SpriteRendererComponent>(dstSceneReg,srcSceneReg,enttMap);
        CopyComponent<CameraComponent>(dstSceneReg,srcSceneReg,enttMap);
        CopyComponent<RigidBodyFlatComponent>(dstSceneReg,srcSceneReg,enttMap);
        CopyComponent<BoxFlatComponent>(dstSceneReg,srcSceneReg,enttMap);
        //CopyComponent<NativeScriptComponent>(dstSceneReg,srcSceneReg,enttMap);

        return newScene;
    }
    void Scene::OnUpdateRuntime(Timestep ts) {

        {
            m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc){
                //Scene On ScenePlay
                if(!nsc.Instance){

                    nsc.Instance = nsc.InstantiateScript();
                    nsc.Instance->m_Entity = {entity, this};
                    nsc.Instance->OnCreate();
                }
                nsc.Instance->OnUpdate(ts);
            });
        }
        // Box2D simulation
        {
            const int32_t velocityIteration = 6;
            const int32_t positionIterations = 2;

            m_world->Step(ts, velocityIteration, positionIterations);
            auto view = m_Registry.view<RigidBodyFlatComponent>();
            for(auto e: view){
                Entity entity = {e, this};
                auto& transform = entity.GetComponent<TransformComponent>();
                auto& rb = entity.GetComponent<RigidBodyFlatComponent>();

                b2Body* body = (b2Body*)rb.RuntimeBody;

                const auto& position = body->GetPosition();
                transform.Translation.x = position.x;
                transform.Translation.y = position.y;
                transform.Rotation.z = body->GetAngle();
            }
        }

        // Render Sprites
        Camera* maincamera = nullptr;
        glm::mat4 cameraTransform;
        {
            auto group = m_Registry.view<CameraComponent, TransformComponent>();
            for (auto entity: group) {
                auto [camera, transform] = group.get<CameraComponent, TransformComponent>(entity);

                if(camera.Primary){
                    maincamera = &camera.Camera;
                    cameraTransform = transform.GetTransform();
                    break;
                }
            }
        }
        if(maincamera) {

            Renderer2D::BeginScene(*maincamera, cameraTransform);

            {

                auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
                for (auto entity: group) {
                    auto[sprite, transform] = group.get<SpriteRendererComponent, TransformComponent>(entity);
                    Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int) entity);
                }
            }

//            {
//
//                auto group = m_Registry.group<TransformComponent>(entt::get<CircleRenderComponent>);
//                for (auto entity: group) {
//                    auto[circle, transform] = group.get<CircleRenderComponent, TransformComponent>(entity);
//                    Renderer2D::DrawCircle(transform.GetTransform(), circle.Color,circle.Thickness,circle.Fade, (int) entity);
//                }
//            }



            Renderer2D::EndScene();

        }
    }
    Entity Scene::CreateEntity(const std::string& name) {
        return CreateEntityWithUUID(UUID(), name);
    }
    Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name){
        Entity entity = {m_Registry.create(), this};
        entity.AddComponent<TransformComponent>();
        entity.AddComponent<IDComponent>(uuid);
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity": name;
        return entity;
    }
    void Scene::OnRuntimeStart() {
        m_world = new b2World({0.0,-9.8});
        auto view = m_Registry.view<RigidBodyFlatComponent>();
        for (auto e: view) {
            Entity entity = {e, this};
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& rb = entity.GetComponent<RigidBodyFlatComponent>();

            b2BodyDef bodyDef;
            bodyDef.type = (b2BodyType)rb.Type;
            bodyDef.position.Set(transform.Translation.x,transform.Translation.y);
            bodyDef.angle = transform.Rotation.z;


            b2Body* body = m_world->CreateBody(&bodyDef);
            body->SetFixedRotation(rb.FixedRotation);
            rb.RuntimeBody = body;

            if(entity.HasComponent<BoxFlatComponent>()){
                auto& bc_flat = entity.GetComponent<BoxFlatComponent>();

                b2PolygonShape shape;

                shape.SetAsBox(
                        bc_flat.Size.x * transform.Scale.x
                        , bc_flat.Size.y * transform.Scale.y);

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &shape;
                fixtureDef.density = bc_flat.Density;
                fixtureDef.friction = bc_flat.Friction;
                fixtureDef.restitution = bc_flat.Bounce;
                fixtureDef.restitutionThreshold = bc_flat.BounceThreshold;
                body->CreateFixture(&fixtureDef);

            }

        }
    }
    void Scene::OnRuntimeStop() {
        delete m_world;
        m_world = nullptr;
    }
    void Scene::DestroyEntity(Entity entity) {
        m_Registry.destroy(entity);
    }
    Entity Scene::DuplicateEntity(Entity& other) {
        return CloneEntityWithUUID(UUID(),other);
    }
    Entity Scene::CloneEntityWithUUID(UUID uuid, Entity &other) {
        Entity entity = {m_Registry.create(), this};
        if(other.HasComponent<SpriteRendererComponent>()){
            entity.AddComponent<SpriteRendererComponent>();
            SpriteRendererComponent sprite = other.GetComponent<SpriteRendererComponent>();
            SpriteRendererComponent newSprite = entity.GetComponent<SpriteRendererComponent>();

            newSprite = sprite;
        }
        return entity;
    }


    void Scene::OnViewportReSize(uint32_t width, uint32_t height) {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        auto view = m_Registry.view<CameraComponent>();
        for(auto entity: view){
            auto& cameraComp = view.get<CameraComponent>(entity);
            if(!cameraComp.FixedAspectRatio){
                cameraComp.Camera.SetViewPortSize(width, height);
            }
        }
    }

    template<typename T>
    void Scene::OnAdded(Entity entity, T& component){
        RC_CORE_ASSERT(false,"Added Entity not recognized");
    }
    template<>
    void Scene::OnAdded<IDComponent>(Entity entity, IDComponent& comp){

    }
    template<>
    void Scene::OnAdded<TransformComponent>(Entity entity, TransformComponent& comp){

    }
    template<>
    void Scene::OnAdded<CameraComponent>(Entity entity, CameraComponent& component){
        if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
            component.Camera.SetViewPortSize(m_ViewportWidth, m_ViewportHeight);
    }
    template<>
    void Scene::OnAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component){

    }
    template<>
    void Scene::OnAdded<TagComponent>(Entity entity, TagComponent& comp){

    }
    template<>
    void Scene::OnAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& comp){

    }
    template<>
    void Scene::OnAdded<BoxFlatComponent>(Entity entity, BoxFlatComponent& comp){

    }
    template<>
    void Scene::OnAdded<RigidBodyFlatComponent>(Entity entity, RigidBodyFlatComponent& comp){

    }
    template<>
    void Scene::OnAdded<CircleRenderComponent>(Entity entity, CircleRenderComponent& comp){

    }
    Entity Scene::GetPrimaryCameraEntity() {
        auto view = m_Registry.view<CameraComponent>();
        for(auto entity: view){
            const auto& camera = view.get<CameraComponent>(entity);
            if(camera.Primary)
                return Entity{entity, this};
        }
        return {};
    }
    void Scene::OnUpdateEditor(Timestep ts, EditorCamera &camera) {

        Renderer2D::BeginScene(camera);

        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity: group) {
            auto [sprite, transform] = group.get<SpriteRendererComponent,TransformComponent>(entity);

            Renderer2D::DrawSprite(transform.GetTransform(),sprite,(int)entity);

        }
        Renderer2D::EndScene();
    }

}