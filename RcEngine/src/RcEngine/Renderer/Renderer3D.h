//
// Created by Tristan Zippert on 7/28/21.
//
#pragma once

#include "RcEngine/Renderer/Camera.h"
#include "RcEngine/Renderer/Texture.h"
#include "RcEngine/Renderer/EditorCamera.h"

#ifndef RCENGINE_RENDERER3D_H
#define RCENGINE_RENDERER3D_H

namespace RcEngine{
    class Renderer3D{
    public:
        static void Init();
        static void Shutdown();

        //TODO: implement perspective camera
        //static void BeginScene(const PerspectiveCamera& camera);
        static void BeginScene(const EditorCamera& camera);
        static void BeginScene(const Camera& camera, const glm::mat4 transform);

        static void EndScene();
        static void Flush();

    };
}

#endif //RCENGINE_RENDERER3D_H
