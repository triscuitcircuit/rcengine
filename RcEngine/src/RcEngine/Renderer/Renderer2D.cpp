//
// Created by Tristan Zippert on 6/24/21.
//
#include "rcpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"

#include "RenderCommand.h"
#include <external/glm/glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


namespace RcEngine{

    struct QuadVertex{
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
        float TilingFactor;
    };

    struct Renderer2DStorage{
        static const uint32_t  MaxQuads = 10000;
        static const uint32_t  MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 16; // Render Capacity. APPLE M1: 16 fragment shaders

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Texture2D> WhiteTexture;
        Ref<Shader> TextureShader;

        uint32_t QuadIndexCount =0;

        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

        std::array<Ref<Texture2D>,MaxTextureSlots> TextureSlots;
        uint32_t  TextureSlotIndex = 1; //0 = base texture

        glm::vec4 QuadVertexPositions[4];

        Renderer2D::Statistics Stats;
    };

    static Renderer2DStorage s_Data;

    void Renderer2D::Init() {
        RC_PROFILE_FUNCTION();


        s_Data.QuadVertexArray = VertexArray::Create();


        s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));

        s_Data.QuadVertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float, "a_TexIndex" },
            { ShaderDataType::Float, "a_TilingFactor" },
            });

        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

        s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

        uint32_t* quadIndices = new uint32_t [s_Data.MaxIndices];

        uint32_t  offset = 0;
        for(uint32_t i =0; i < s_Data.MaxIndices; i+=6){
            quadIndices[i+0] = offset + 0;
            quadIndices[i+1] = offset + 1;
            quadIndices[i+2] = offset + 2;

            quadIndices[i+3] = offset + 2;
            quadIndices[i+4] = offset + 3;
            quadIndices[i+5] = offset + 0;

            offset += 4;
        }

        RcEngine::Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices,
                                    s_Data.MaxIndices);

        s_Data.QuadVertexArray ->SetIndexBuffer(quadIB);

        delete[] quadIndices;

        s_Data.WhiteTexture = Texture2D::Create(1,1);
        uint32_t blankData = 0xffffffff;
        s_Data.WhiteTexture->SetData(&blankData, sizeof(uint32_t));

        int32_t samplers[s_Data.MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
            samplers[i] = i;

        s_Data.TextureShader = Shader::Create("Assets/Shaders/TextureCombined.glsl");
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetIntArray("u_Textures",samplers,s_Data.MaxTextureSlots);

        s_Data.TextureSlots[0] = s_Data.WhiteTexture;

        s_Data.QuadVertexPositions[0] = {-0.5, -0.5,0.0f,1.0f};
        s_Data.QuadVertexPositions[1] = {0.5, -0.5,0.0f,1.0f};
        s_Data.QuadVertexPositions[2] = {0.5, 0.5,0.0f,1.0f};
        s_Data.QuadVertexPositions[3] = {-0.5, 0.5,0.0f,1.0f};
    }
    void Renderer2D::BeginScene(const OrthoCamera &camera) {
        RC_PROFILE_FUNCTION();

        s_Data.TextureShader->Bind();
        s_Data.TextureShader->
                SetMat4("u_ViewProjection",camera.GetViewProjectMatrix());

        s_Data.QuadIndexCount =0;

        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        s_Data.TextureSlotIndex = 1;

    }
    void Renderer2D::BeginScene(const Camera &camera, const glm::mat4 &transform) {
        RC_PROFILE_FUNCTION();

        glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

        s_Data.TextureShader->Bind();
        s_Data.TextureShader->
                SetMat4("u_ViewProjection",viewProj);

        s_Data.QuadIndexCount =0;

        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        s_Data.TextureSlotIndex = 1;
    }
    void Renderer2D::EndScene() {
        RC_PROFILE_FUNCTION();

        uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
        Flush();
    }

    void Renderer2D::Flush() {
        //Bind Textures
        for(uint32_t i =0; i < s_Data.TextureSlotIndex; i++)
            s_Data.TextureSlots[i]->Bind(i);

        RenderCommand::DrawIndexed(s_Data.QuadVertexArray,s_Data.QuadIndexCount);
        s_Data.Stats.DrawCalls++;
    }
    void Renderer2D::StartNewBatch() {
        EndScene();

        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        s_Data.TextureSlotIndex = 1;
    }

    void Renderer2D::Shutdown() {}

    void Renderer2D::DrawQuad(const glm::vec2 &position,
                              const glm::vec2 &size,
                              const glm::vec4 &color, float tilingfactor) {
        RC_PROFILE_FUNCTION();

        DrawQuad({position.x,position.y,0.0f},size,color,tilingfactor);

    }
    void Renderer2D::DrawQuad(const glm::vec3 &position,
                              const glm::vec2 &size,
                              const glm::vec4 &color,float tilingfactor) {
        RC_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f),position)
                              *glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});

        DrawQuad(transform,color,tilingfactor);

#if NON_BATCH
        s_Data.TextureShader->
                SetFloat("u_TileMultiplier", tilingfactor);

        s_Data.WhiteTexture->Bind();

        glm::mat4 transform  = glm::translate(glm::mat4(1.0f),position)*
                               glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});

        s_Data.TextureShader->
                SetMat4("u_Transform",transform);

        s_Data.QuadVertexArray->Bind();

        RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
#endif
    }
    void Renderer2D::DrawQuad(const glm::vec2 &position,
                              const glm::vec2 &size,const glm::vec4& color,
                              const Ref<Texture2D>& texture,float tilingfactor) {
        RC_PROFILE_FUNCTION();

        DrawQuad({position.x,position.y,0.0f},size,color,texture, tilingfactor);

    }
    void Renderer2D::DrawQuad(const glm::vec3 &position,
                              const glm::vec2 &size,const glm::vec4& color,
                              const Ref<Texture2D>& texture,float tilingfactor) {
        RC_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f),position)
                              *glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});

        DrawQuad(transform,color,texture,tilingfactor);
#if NON_BATCH
        s_Data.TextureShader->
                SetFloat4("u_Color", color);

        s_Data.TextureShader->
                SetFloat("u_TileMultiplier", tilingfactor);

        texture->Bind();
        glm::mat4 transform  = glm::translate(glm::mat4(1.0f),position)*
                               glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});

        s_Data.TextureShader->
                SetMat4("u_Transform",transform);


        s_Data.QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
#endif
    }
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2 &size, const glm::vec4 &color,
                              const Ref <SubTexture2D> &subtexture, float tilingfactor) {
        DrawQuad({position.x,position.y,0.0f},size,color,subtexture, tilingfactor);
    }
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
                                 const Ref<SubTexture2D>& subtexture,float tilingfactor) {
        RC_PROFILE_FUNCTION();

        constexpr size_t QuadVertexCount = 4;

        const glm::vec2 *textureCoords = subtexture->GetTexCoords();
        const Ref <Texture2D> texture = subtexture->GetTexture();

        if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices) {
            StartNewBatch();
        }

        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
            if (*s_Data.TextureSlots[i].get() == *texture.get()) {
                textureIndex = (float) i;
                break;
            }
        }
        if (textureIndex == 0.0f) {
            textureIndex = (float) s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                              * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});


        for (size_t i = 0; i < QuadVertexCount; i++) {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor = tilingfactor;
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;
        s_Data.Stats.QuadCount++;

    }
    void Renderer2D::DrawQuad(const glm::mat4 transform, const glm::vec4 color, float tilingfactor) {
        RC_PROFILE_FUNCTION();

        if(s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices){
            StartNewBatch();
        }

        const float texIndex = 0.0f;
        constexpr size_t QuadVertexCount = 4;

        constexpr glm::vec2 textureCoords[] =
                {
                        {0.0f,0.0f},
                        {1.0f,0.0f},
                        {1.0f,1.0f},
                        {0.0f,1.0f}
                };

        for(int i =0; i< QuadVertexCount; i++){
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor = tilingfactor;
            s_Data.QuadVertexBufferPtr++;
        }
        s_Data.QuadIndexCount+=6;
        s_Data.Stats.QuadCount++;

    }
    void Renderer2D::DrawQuad(const glm::mat4 transform, const glm::vec4 color, const Ref <Texture2D> &texture,
                              float tilingfactor) {
        RC_PROFILE_FUNCTION();

        constexpr size_t QuadVertexCount = 4;

        constexpr glm::vec2 textureCoords[] =
                {
                        {0.0f,0.0f},
                        {1.0f,0.0f},
                        {1.0f,1.0f},
                        {0.0f,1.0f}
                };



        if(s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices){
            StartNewBatch();
        }

        float textureIndex = 0.0f;
        for(uint32_t i = 1; i < s_Data.TextureSlotIndex; i++){
            if(*s_Data.TextureSlots[i].get() == *texture.get()){
                textureIndex = (float)i;
                break;
            }
        }
        if(textureIndex == 0.0f){
            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }

        for(size_t i =0; i< QuadVertexCount; i++){
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor = tilingfactor;
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount+=6;
        s_Data.Stats.QuadCount++;
    }
//-------------------------------------------------------------------
//Rotated Quad Draw code
//-------------------------------------------------------------------
    /// Draws a Quad with color, and rotation in radians
    void Renderer2D::DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation,
                                     const glm::vec4 &color, float tilingfactor) {
        RC_PROFILE_FUNCTION();
        DrawRotatedQuad({position.x,position.y,0.0f},size, rotation,color,tilingfactor);
    }
    /// Draws a Quad with color, and rotation in radians
    void Renderer2D::DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation,
                                     const glm::vec4 &color, float tilingfactor) {
        RC_PROFILE_FUNCTION();

        if(s_Data.QuadIndexCount > Renderer2DStorage::MaxIndices){
            StartNewBatch();
        }


        const float texIndex = 0.0f;

        glm::mat4 transform = glm::translate(glm::mat4(1.0f),position) *
                              glm::rotate(glm::mat4(1.0f),rotation, {0.0f,0.0f,1.0f})
                              *glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});

        constexpr size_t QuadVertexCount = 4;
        constexpr glm::vec2 textureCoords[] =
                {
                        {0.0f,0.0f},
                        {1.0f,0.0f},
                        {1.0f,1.0f},
                        {0.0f,1.0f}
                };


        for(int i =0; i< QuadVertexCount; i++){
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor = tilingfactor;
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount+=6;
        s_Data.Stats.QuadCount++;



#if NON_BATCH
        s_Data.TextureShader->
                SetFloat4("u_Color", color);
        s_Data.TextureShader->
                SetFloat("u_TileMultiplier", tilingfactor);

        glm::mat4 transform  = glm::translate(glm::mat4(1.0f),position)*
                glm::rotate(glm::mat4(1.0f),rotation,{0.0f,0.0f,1.0f})
                *glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});

        s_Data.TextureShader->
                SetMat4("u_Transform",transform);

        s_Data.QuadVertexArray->Bind();
        s_Data.WhiteTexture->Bind();
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
#endif

    }
    /// Draws a Quad with texture, color and rotation in radians
    void Renderer2D::DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color,
                                     float rotation, const Ref <Texture2D> &texture, float tilingfactor) {
        RC_PROFILE_FUNCTION();
        DrawRotatedQuad({position.x,position.y,0.0f},size,color,rotation, texture, tilingfactor);
    }
    /// Draws a Quad with texture, color and rotation in radians
    void Renderer2D::DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color,
                                     float rotation, const Ref <Texture2D> &texture, float tilingfactor) {

        float textureIndex = 0.0f;

        if(s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices){
            StartNewBatch();
        }

        for(uint32_t i = 1; i < s_Data.TextureSlotIndex; i++){
            if(*s_Data.TextureSlots[i].get() == *texture.get()){
                textureIndex = (float)i;
                break;
            }
        }
        if(textureIndex == 0.0f){
            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }
        glm::mat4 transform = glm::translate(glm::mat4(1.0f),position) *
                              glm::rotate(glm::mat4(1.0f),rotation, {0.0f,0.0f,1.0f})
                              *glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});

        constexpr size_t QuadVertexCount = 4;
        constexpr glm::vec2 textureCoords[] =
                {
                        {0.0f,0.0f},
                        {1.0f,0.0f},
                        {1.0f,1.0f},
                        {0.0f,1.0f}
                };

        for(int i =0; i< QuadVertexCount; i++){
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor = tilingfactor;
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount+=6;
        s_Data.Stats.QuadCount++;

#if NON_BATCH
        s_Data.TextureShader->
                SetFloat4("u_Color", color);
        s_Data.TextureShader->
                SetFloat("u_TileMultiplier", tilingfactor);

        texture->Bind();
        glm::mat4 transform  = glm::translate(glm::mat4(1.0f),position)*
                glm::rotate(glm::mat4(1.0f),rotation,{0.0f,0.0f,1.0f})
                *glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});

        s_Data.TextureShader->
                SetMat4("u_Transform",transform);


        s_Data.QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
#endif
    }
    void Renderer2D::DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color,
                                     float rotation, const Ref <SubTexture2D> &subtexture, float tilingfactor) {
        RC_PROFILE_FUNCTION();
        DrawRotatedQuad({position.x,position.y,0.0f},size,color,rotation,subtexture,tilingfactor);
    }
    void Renderer2D::DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color,
                                     float rotation, const Ref <SubTexture2D> &subtexture, float tilingfactor) {
        RC_PROFILE_FUNCTION();



        constexpr size_t QuadVertexCount = 4;

        const glm::vec2* textureCoords = subtexture->GetTexCoords();
        const Ref<Texture2D> texture = subtexture->GetTexture();

        if(s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices){
            StartNewBatch();
        }

        float textureIndex = 0.0f;
        for(uint32_t i = 1; i < s_Data.TextureSlotIndex; i++){
            if(*s_Data.TextureSlots[i].get() == *texture.get()){
                textureIndex = (float)i;
                break;
            }
        }
        if(textureIndex == 0.0f){
            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f),position) *
                              glm::rotate(glm::mat4(1.0f),rotation, {0.0f,0.0f,1.0f})
                              *glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});

        for(size_t i =0; i< QuadVertexCount; i++){
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor = tilingfactor;
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount+=6;
        s_Data.Stats.QuadCount++;


    }

    void Renderer2D::ResetStats(){
        memset(&s_Data.Stats,0, sizeof(Statistics));
    }
    Renderer2D::Statistics Renderer2D::GetStats() {
        return s_Data.Stats;
    }


}