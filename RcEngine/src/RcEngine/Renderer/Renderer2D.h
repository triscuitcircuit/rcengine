//
// Created by Tristan Zippert on 6/24/21.
//
#pragma once

#ifndef RCENGINE_CLION_RENDERER2D_H
#define RCENGINE_CLION_RENDERER2D_H

#include "RcEngine/Renderer/OrthoCamera.h"
#include "RcEngine/Renderer/Camera.h"
#include "RcEngine/Renderer/Texture.h"
#include "RcEngine/Renderer/SubTexture2D.h"

#include "RcEngine/Renderer/EditorCamera.h"
#include <RcEngine/Scene/Component.h>

namespace RcEngine{
    class Renderer2D{
    public:

        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthoCamera& camera );
        static void BeginScene(const EditorCamera& camera);
        static void BeginScene(const Camera& camera, const glm::mat4& transform);

        static void EndScene();
        static void Flush();

        // DRAWING PRIMITIVE SHAPES
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size,
                             const glm::vec4& color, float tilingfactor=1.0f);

        static void DrawQuad(const glm::vec3& position, const glm::vec2& size,
                             const glm::vec4& color,float tilingfactor=1.0f);

        static void DrawQuad(const glm::mat4& transform, const glm::vec4& color,
                             float tilingfactor=1.0f, int entityID = -1);
        // overloaded position functions
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size,const glm::vec4& color,
                             const Ref<Texture2D>& texture, float tilingfactor=1.0f);

        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
                             const Ref<Texture2D>& texture,float tilingfactor=1.0f);

        /////
        static void DrawQuad(const glm::mat4& transform, const glm::vec4& color,
                             const Ref<Texture2D>& texture,float tilingfactor=1.0f ,int entityID =-1);

        //Sprite-Sheet and Sub texture calls-------------------------
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size,const glm::vec4& color,
                             const Ref<SubTexture2D>& subtexture, float tilingfactor=1.0f);

        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
                             const Ref<SubTexture2D>& subtexture,float tilingfactor=1.0f);
        //-----------------------------------------------------------

        static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);        // Rotated Quad draw functions to avoid non-warranted matrix math
        /// Draws a Quad with color, and rotation in radians
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size,
                             float rotation,
                             const glm::vec4& color, float tilingfactor=1.0f);

        /// Draws a Quad with color, and rotation in radians
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size,
                             float rotation,
                             const glm::vec4& color,float tilingfactor=1.0f);

        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size,const glm::vec4& color,
                                    float rotation,
                             const Ref<Texture2D>& texture, float tilingfactor=1.0f);
        /// Draws a Quad with texture, color and rotation in radians
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
                                    float rotation,
                             const Ref<Texture2D>& texture,float tilingfactor=1.0f);


        //Sprite-Sheet and Sub texture calls-------------------------
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size,const glm::vec4& color,
                                    float rotation,
                                    const Ref<SubTexture2D>& subtexture, float tilingfactor=1.0f);

        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
                                    float rotation,
                                    const Ref<SubTexture2D>& subtexture,float tilingfactor=1.0f);
        //-----------------------------------------------------------

        static void DrawSquare(const glm::vec3& position, const glm::vec4& color);

        static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness= 1.0f, float fade = 0.005f, int entityID = -1);

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
        static void StartBatch();
    };
}
#endif //RCENGINE_CLION_RENDERER2D_H
