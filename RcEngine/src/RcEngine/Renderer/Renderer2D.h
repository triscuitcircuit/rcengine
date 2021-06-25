//
// Created by Tristan Zippert on 6/24/21.
//
#pragma once
#include "RcEngine/Renderer/OrthoCamera.h"
#include "RcEngine/Renderer/Texture.h"

#ifndef RCENGINE_CLION_RENDERER2D_H
#define RCENGINE_CLION_RENDERER2D_H
namespace RcEngine{
    class Renderer2D{
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthoCamera& camera );
        static void EndScene();
        // DRAWING PRIMITIVE SHAPES
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        // Overloaded rotation Quad draw
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size,const glm::vec4& color, const Ref<Texture2D>& texture);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const Ref<Texture2D>& texture);
    };
}
#endif //RCENGINE_CLION_RENDERER2D_H
