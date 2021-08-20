//
// Created by Tristan Zippert on 7/30/21.
//
#include "rcpch.h"
#include "ContentBrowserPanel.h"
#include "Platform/MacUtils.h"

#include <external/imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <external/imgui/imgui_internal.h>

namespace RcEngine{

    extern const std::filesystem::path g_AssetPath = "Assets";

    ContentBrowserPanel::ContentBrowserPanel()
    : m_CurrentDirectory(g_AssetPath){
        m_DirectoryIcon = Texture2D::Create("Assets/Icons/ContentBrowser/DirectoryIcon.png");
        m_FileIcon = Texture2D::Create("Assets/Icons/ContentBrowser/FileIcon.png");
    }

    void ContentBrowserPanel::OnImGuiRender()
    {
        ImGui::Begin("Content Browser");

        if(ImGui::BeginPopupContextWindow(nullptr,1,false)){
            if(ImGui::MenuItem("Open in file explorer")){
                FileDialogs::OpenExplorer(m_CurrentDirectory.c_str());
            }
            ImGui::EndPopup();
        }

        if(m_CurrentDirectory != std::filesystem::path(g_AssetPath)){
            if(ImGui::Button("<-")){
                m_CurrentDirectory = m_CurrentDirectory.parent_path();
            }
        }
        ImGui::TextWrapped("Directory: %s",m_CurrentDirectory.c_str());
        ImGui::Separator();
        static float padding = 16.0f;
        static float thumbnailSize = 128.0f;
        float cellSize = thumbnailSize + padding;

        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = (int)(panelWidth/cellSize);

        if(columnCount < 1)
            columnCount = 1;

        ImGui::Columns(columnCount,nullptr, false);

        for(auto& directoryEntry: std::filesystem::directory_iterator(m_CurrentDirectory)){
            const auto& path = directoryEntry.path();
            auto relativePath = std::filesystem::relative(path, g_AssetPath);
            std::string filenameString = relativePath.filename().string();

            ImGui::PushID(filenameString.c_str());
            Ref<Texture2D> icon = directoryEntry.is_directory()? m_DirectoryIcon: m_FileIcon;

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
            ImGui::ImageButton((ImTextureID)icon->GetRendererID(),
                               {thumbnailSize, thumbnailSize},{0,1},{1,0});

            if(ImGui::BeginDragDropSource()){
                std::string w_stringPath  = relativePath.string();
                std::wstring w_relativePath = std::wstring(w_stringPath.begin(),w_stringPath.end());
                const wchar_t* itemPath = w_relativePath.c_str();
                ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM",itemPath,
                                          (wcslen(itemPath)+1)* sizeof(wchar_t));
                ImGui::Text("%s", relativePath.c_str());
                ImGui::EndDragDropSource();
            }
            ImGui::PopStyleColor();

            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                if (directoryEntry.is_directory())
                    m_CurrentDirectory /= path.filename();

            }
            ImGui::TextWrapped("%s",filenameString.c_str());

            ImGui::NextColumn();

            ImGui::PopID();


        }
        ImGui::Columns(1);

        ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
        ImGui::SliderFloat("Padding", &padding, 0, 32);

        ImGui::End();
    }
}
