//
// Created by Tristan Zippert on 7/6/21.
//
#include "rcpch.h"
#include "Scene.h"
#include "Component.h"

#include "Entity.h"

#include "RcEngine/Renderer/Renderer2D.h"


namespace RcEngine{
    Scene::Scene() {
    }
    Scene::~Scene() {

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

            Renderer2D::BeginScene(maincamera->GetProjection(), cameraTransform);


            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity: group) {
                auto [sprite, transform] = group.get<SpriteRendererComponent,TransformComponent>(entity);
                Renderer2D::DrawSprite(transform.GetTransform(),sprite,(int)entity);
            }
            Renderer2D::EndScene();

        }
    }
    Entity Scene::CreateEntity(const std::string& name) {
        Entity entity = {m_Registry.create(), this};
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity": name;
        return entity;
    }
    void Scene::DestroyEntity(Entity entity) {
        m_Registry.destroy(entity);
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
    void Scene::OnAdded<TransformComponent>(Entity entity, TransformComponent& comp){

    }
    template<>
    void Scene::OnAdded<CameraComponent>(Entity entity, CameraComponent& camera){
        camera.Camera.SetViewPortSize(m_ViewportWidth, m_ViewportHeight);
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