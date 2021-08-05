//
// Created by Tristan Zippert on 7/8/21.
//

#include "SceneHierarchyPanel.h"
#include "RcEngine/Scene/Component.h"


#include <external/imgui/imgui.h>
#include <gtc/type_ptr.hpp>
#include <external/imgui/imgui_internal.h>

namespace RcEngine{
    //extern const std::filesystem::path g_AssetPath;

    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed| ImGuiSelectableFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap;
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene> &scene) {
        SetContext(scene);
    }
    void SceneHierarchyPanel::SetContext(const Ref<Scene> &scene) {
        m_Scene = scene;
        m_Selected = {};
    }
    void SceneHierarchyPanel::OnImGuiRender() {
        ImGui::Begin("Scene Hierarchy");

        m_Scene->m_Registry.each([&](auto entityID){
            Entity entity {entityID, m_Scene.get()};

            DrawEntityNode(entity);

        });
        if(ImGui::IsMouseDown(0)&& ImGui::IsWindowHovered()){
            m_Selected = {};
        }
        // Right click blank space
        if(ImGui::BeginPopupContextWindow(0,1,false)){
            if(ImGui::MenuItem("Create new Entity")){
                m_Scene->CreateEntity("Empty Entity");
            }
            ImGui::EndPopup();
        }

        ImGui::End();

        ImGui::Begin("Properties");
        if(m_Selected){
            DrawComponents(m_Selected);

            if(ImGui::Button("Add Component"))
                ImGui::OpenPopup("AddComponent");

            if(ImGui::BeginPopup("AddComponent")){

                if(ImGui::MenuItem("Camera")){
                    m_Selected.AddComponent<CameraComponent>();

                    ImGui::CloseCurrentPopup();
                }
                if(ImGui::MenuItem("Sprite")){
                    m_Selected.AddComponent<SpriteRendererComponent>();

                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }

        }
        ImGui::End();
    }
    void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
        auto& tc = entity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = ((m_Selected == entity)? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity,flags, "%s", tc.c_str());
        if(ImGui::IsItemClicked()){
            m_Selected = entity;
        }
        bool entityDeleted = false;
        if(ImGui::BeginPopupContextItem()){
            if(ImGui::MenuItem("Destroy Entity")){
                entityDeleted = true;
            }
            ImGui::EndPopup();
        }

        if(opened){
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
            bool opened = ImGui::TreeNodeEx((void*)9817239, flags, "%s", tc.c_str());
            if(opened)
                ImGui::TreePop();

            ImGui::TreePop();
        }
        if(entityDeleted){
            m_Scene->DestroyEntity(entity);
            if(m_Selected ==entity){
                m_Selected = {};
            }
        }
    }

    static void DrawVec3Control(const std::string& label, glm::vec3& values,
                                float resetValue =0.0f, float colWidth = 100.0f){
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];

        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, colWidth);

        ImGui::Text("%s", label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0,0});

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

        ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

        ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{0.8f,0.1f,0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{0.9f,0.2f,0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{0.8f,0.1f,0.15f, 1.0f});

        ImGui::PushFont(boldFont);
        if(ImGui::Button("X", buttonSize))
            values.x =  resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X",&values.x,0.1f,0.0f,0.0f,"%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{0.2f,0.7f,0.3f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{0.2f,0.2f,0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{0.2f,0.7f,0.15f, 1.0f});

        ImGui::PushFont(boldFont);
        if(ImGui::Button("Y", buttonSize))
            values.y =  resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y",&values.y,0.1f,0.0f,0.0f,"%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{0.1f,0.25f,0.8f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{0.2f,0.35f,0.9f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{0.1f,0.25f,0.8f, 1.0f});

        ImGui::PushFont(boldFont);
        if(ImGui::Button("Z", buttonSize))
            values.z =  resetValue;
        ImGui::PopFont();

        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z",&values.z,0.1f,0.0f,0.0f,"%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();


        ImGui::Columns(1);
        ImGui::PopID();

    }

    template<typename T, typename UIFunction>
    //for some reason C++ doesnt automatically convert std::string to const char*
    static void DrawComponent(const char*  name, Entity entity, UIFunction uifunction){
        if(entity.HasComponent<T>()){

            auto& component = entity.GetComponent<T>();
            ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4,4});

            bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(),treeNodeFlags,  "%s", name);

            ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
            if(ImGui::Button("+", ImVec2{20,20})){
                ImGui::OpenPopup("ComponentSettings");
            }
            ImGui::PopStyleVar();

            bool removeComp = false;
            if(ImGui::BeginPopup("ComponentSettings")){
                if(ImGui::MenuItem("Remove Component"))
                    removeComp = true;
                ImGui::EndPopup();
            }

            if(open){
                uifunction(component);
                ImGui::TreePop();
            }
            if(removeComp)
                entity.RemoveComponent<T>();
        }
    }

    void SceneHierarchyPanel::DrawComponents(Entity entitySelection) {

        if(entitySelection.HasComponent<TagComponent>()){
            auto& tc = entitySelection.GetComponent<TagComponent>().Tag;

            char buffer[256];
            memset(buffer,0,sizeof(buffer));
            strcpy(buffer, tc.c_str());
            if(ImGui::InputText("Tag",buffer, sizeof(buffer))){
                tc = std::string(buffer);
            }
        }
        DrawComponent<TransformComponent>("Transform",entitySelection, [](auto& tc){
            DrawVec3Control("Translation", tc.Translation);
            glm::vec3 rotation = glm::degrees(tc.Rotation);

            DrawVec3Control("Rotation", rotation);

            tc.Rotation = glm::radians(rotation);
            DrawVec3Control("Scale", tc.Scale,1.0f);

        });
        DrawComponent<CameraComponent>("Camera",entitySelection, [](auto& component){

            auto& camera = component.Camera;

            ImGui::Checkbox("Primary",&component.Primary);

            const char* projectionTypeStrings[] = {"Perspective", "Orthographic", "Isometric"};
            const char* currentProjectionTypeString = projectionTypeStrings[(int)component.Camera.GetProjectionType()];



            if(ImGui::BeginCombo("Projection",currentProjectionTypeString)){

                for(int i =0; i< 3; ++i){
                    bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                    if(ImGui::Selectable(projectionTypeStrings[i], isSelected)){
                        currentProjectionTypeString = projectionTypeStrings[i];
                        camera.SetProjectionType((SceneCamera::ProjectionType)i);
                    }

                    if(isSelected){
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }

            float orthosize = camera.GetOrthographicSize();
            float orthonear = camera.GetOrthoNearClip();
            float orthofar = camera.GetOrthoFarClip();

            float isosize = camera.GetIsoSize();
            float isonear = camera.GetIsoNearClip();
            float isofar = camera.GetIsoFarClip();

            float perspfov = glm::degrees(camera.GetPerspectiveVerticalFov());
            float perspnear = camera.GetPerspectiveNearClip();
            float persfar = camera.GetPerspectiveFarClip();


            switch (camera.GetProjectionType()) {
                case SceneCamera::ProjectionType::Perspective:

                    if(ImGui::DragFloat("Fov",&perspfov)){
                        camera.SetPerspectiveVerticalFov(glm::radians(perspfov));}

                    if(ImGui::DragFloat("Near",&perspnear)){
                        camera.SetPerspectiveNearClip(perspnear);}

                    if(ImGui::DragFloat("Far",&persfar)){
                        camera.SetPerspectiveFarClip(persfar);}

                    break;
                case SceneCamera::ProjectionType::Ortho:

                    if(ImGui::DragFloat("Size",&orthosize)){
                        camera.SetOrthographicSize(orthosize);}

                    if(ImGui::DragFloat("Near",&orthonear)){
                        camera.SetOrthoNearClip(orthonear);}

                    if(ImGui::DragFloat("Far",&orthofar)){
                        camera.SetOrthoFarClip(orthofar);}

                    ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);

                    break;
                case SceneCamera::ProjectionType::Iso:
                    if(ImGui::DragFloat("Size",&isosize)){
                        camera.SetIsoSize(isosize);}

                    if(ImGui::DragFloat("Near",&isonear)){
                        camera.SetIsoNearClip(isonear);}

                    if(ImGui::DragFloat("Far",&isofar)){
                        camera.SetIsoFarClip(isofar);}

                    break;
            }
        });
        DrawComponent<SpriteRendererComponent>("Sprite Renderer",entitySelection,[](auto& comp){
            ImGui::ColorEdit4("Color",glm::value_ptr(comp.Color));
            //texture

            ImGui::Button("Texture",ImVec2(100.0f, 0.0f));
//            if(ImGui::BeginDragDropTarget()){
//                if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET")){
//                    const auto* path = (const wchar_t *)payload->Data;
//                    std::filesystem::path texturePath = std::filesystem::path(g_AssetPath)/path;
//                    comp.Texture = Texture2D::Create(texturePath.c_str());
//                }
//                ImGui::EndDragDropTarget();
//            }

            ImGui::DragFloat("Tiling Factor",&comp.TilingFactor, 0.1f, 0.0f, 100.0f);

        });

    }

}