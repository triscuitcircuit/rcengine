//
// Created by Tristan Zippert on 7/30/21.
//
#pragma once
#include <filesystem>
#include "RcEngine/Renderer/Texture.h"

#ifndef RCENGINE_CONTENTBROWSERPANEL_H
#define RCENGINE_CONTENTBROWSERPANEL_H
namespace RcEngine{
    class ContentBrowserPanel{
    public:
        ContentBrowserPanel();
        void OnImGuiRender();
    private:
        std::filesystem::path m_CurrentDirectory;

        Ref<Texture2D> m_DirectoryIcon;
        Ref<Texture2D> m_FileIcon;


    };
}
#endif //RCENGINE_CONTENTBROWSERPANEL_H
