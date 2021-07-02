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
        static void Flush();
        // DRAWING PRIMITIVE SHAPES
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size,
                             const glm::vec4& color, float tilingfactor=1.0f);

        static void DrawQuad(const glm::vec3& position, const glm::vec2& size,
                             const glm::vec4& color,float tilingfactor=1.0f);
        // overloaded position functions
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size,const glm::vec4& color,
                             const Ref<Texture2D>& texture, float tilingfactor=1.0f);

        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
                             const Ref<Texture2D>& texture,float tilingfactor=1.0f);

        // Rotated Quad draw functions to avoid non-warranted matrix math

        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size,
                             float rotation,
                             const glm::vec4& color, float tilingfactor=1.0f);

        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size,
                             float rotation,
                             const glm::vec4& color,float tilingfactor=1.0f);

        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size,const glm::vec4& color,
                                    float rotation,
                             const Ref<Texture2D>& texture, float tilingfactor=1.0f);

        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
                                    float rotation,
                             const Ref<Texture2D>& texture,float tilingfactor=1.0f);


        struct Statistics{
            uint32_t  DrawCalls =0;
            uint32_t  QuadCount =0;

            uint32_t  GetTotalVertexCount(){return QuadCount*4;}
            uint32_t  GetTotalIndexCount(){return QuadCount*6;}
        };
        static Statistics GetStats();
        static void ResetStats();
    private:
        static void StartNewBatch();
    };
}
#endif //RCENGINE_CLION_RENDERER2D_H
