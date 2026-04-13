//
// Created by Tristan Zippert on 6/24/21.
//
#include "rcpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"

#include "RenderCommand.h"
#include "UniformBuffer.h"
#include <../glm/glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective



namespace RcEngine{

    struct QuadVertex{
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
        float TilingFactor;

        //Editor
        int EntityID;
    };
    struct CircleVertex{
        glm::vec3 WorldPosition;
        glm::vec3 LocalPosition;
        glm::vec4 Color;
        float Thickness;
        float Fade;

        //Editor
        int EntityID;
    };

    struct LineVertex{
        glm::vec3 Position;
        glm::vec4 Color;

        //Editor
        int EntityID;
    };

    struct Renderer2DStorage{
        static const uint32_t  MaxQuads = 10000;
        static const uint32_t  MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 16; // Render Capacity. APPLE M1: 16 fragment shaders

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Texture2D> WhiteTexture;
        Ref<Shader> QuadShader;

        Ref<VertexArray> CircleVertexArray;
        Ref<VertexBuffer> CircleVertexBuffer;
        Ref<Shader> CircleShader;

        uint32_t QuadIndexCount =0;

        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

        uint32_t CircleIndexCount =0;

       CircleVertex* CircleVertexBufferBase = nullptr;
        CircleVertex* CircleVertexBufferPtr = nullptr;

        std::array<Ref<Texture2D>,MaxTextureSlots> TextureSlots;
        uint32_t  TextureSlotIndex = 1; //0 = base texture

        glm::vec4 QuadVertexPositions[4];

        Renderer2D::Statistics Stats;

        struct CameraData{
            glm::mat4 ViewProjection;
        };
        CameraData CameraBuffer;
        Ref<UniformBuffer> CameraUniformBuffer;
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
            { ShaderDataType::Int, "a_EntityID" },
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
                                    RcEngine::Renderer2DStorage::MaxIndices);

        s_Data.QuadVertexArray ->SetIndexBuffer(quadIB);

        delete[] quadIndices;

        s_Data.CircleVertexArray = VertexArray::Create();
        s_Data.CircleVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CircleVertex));

//        s_Data.CircleVertexBuffer->SetLayout({
//           { ShaderDataType::Float3, "a_WorldPosition" },
//           { ShaderDataType::Float3, "a_LocalPosition" },
//           { ShaderDataType::Float4, "a_Color" },
//           { ShaderDataType::Float2, "a_Thickness" },
//           { ShaderDataType::Float, "a_Fade" },
//           { ShaderDataType::Int, "a_EntityID" },
//           });

//        s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
//        s_Data.CircleVertexArray ->SetIndexBuffer(quadIB);
//        s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];
//

        s_Data.WhiteTexture = Texture2D::Create(1,1);
        uint32_t blankData = 0xffffffff;
        s_Data.WhiteTexture->SetData(&blankData, sizeof(uint32_t));

        int32_t samplers[RcEngine::Renderer2DStorage::MaxTextureSlots];
        for (uint32_t i = 0; i < RcEngine::Renderer2DStorage::MaxTextureSlots; i++)
            samplers[i] = i;

        s_Data.QuadShader = Shader::Create("Assets/Shaders/TextureCombined.glsl");
        s_Data.QuadShader->Bind();
        s_Data.QuadShader->SetIntArray("u_Textures",samplers,s_Data.MaxTextureSlots);

        s_Data.CircleShader = Shader::Create("Assets/Shaders/CircleCombined.glsl");

        s_Data.TextureSlots[0] = s_Data.WhiteTexture;

        s_Data.QuadVertexPositions[0] = {-0.5, -0.5,0.0f,1.0f};
        s_Data.QuadVertexPositions[1] = {0.5, -0.5,0.0f,1.0f};
        s_Data.QuadVertexPositions[2] = {0.5, 0.5,0.0f,1.0f};
        s_Data.QuadVertexPositions[3] = {-0.5, 0.5,0.0f,1.0f};

        s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DStorage::CameraData),0);

    }
    void Renderer2D::BeginScene(const OrthoCamera &camera) {
        RC_PROFILE_FUNCTION();

        s_Data.CameraBuffer.ViewProjection = camera.GetProjectionMatrix();
        s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DStorage::CameraData));

        StartBatch();

    }
    void Renderer2D::BeginScene(const Camera &camera, const glm::mat4 &transform) {
        RC_PROFILE_FUNCTION();

        s_Data.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
        //s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DStorage::CameraData));

        s_Data.QuadShader->Bind();
        s_Data.QuadShader->
                SetMat4("u_ViewProjection",s_Data.CameraBuffer.ViewProjection);
        StartBatch();
    }
    void Renderer2D::StartBatch(){
        s_Data.QuadIndexCount =0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

        s_Data.CircleIndexCount =0;
//        s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

        s_Data.TextureSlotIndex = 1;
    }

    void Renderer2D::EndScene() {
        RC_PROFILE_FUNCTION();

        Flush();
    }

    void Renderer2D::Flush() {

        if(s_Data.QuadIndexCount > 0  ){
            uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
            s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase,dataSize);

            //Bind Textures
        for(uint32_t i =0; i < s_Data.TextureSlotIndex; i++)
            s_Data.TextureSlots[i]->Bind(i);

            s_Data.QuadShader->Bind();

            RenderCommand::DrawIndexed(s_Data.QuadVertexArray,s_Data.QuadIndexCount);
            s_Data.Stats.DrawCalls++;
        }
        if(s_Data.CircleIndexCount > 0){
//            uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
//            s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase,dataSize);
//
//            s_Data.CircleShader->Bind();
//
//            RenderCommand::DrawIndexed(s_Data.CircleVertexArray,s_Data.CircleIndexCount);
            s_Data.Stats.DrawCalls++;
        }
    }
    void Renderer2D::StartNewBatch() {
        EndScene();

        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

        s_Data.CircleIndexCount = 0;
//        s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

        s_Data.TextureSlotIndex = 1;
    }

    void Renderer2D::Shutdown() {
        RC_PROFILE_FUNCTION()

        delete[] s_Data.QuadVertexBufferBase;
    }

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
    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, float tilingfactor,
                              int entityID) {
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
            s_Data.QuadVertexBufferPtr->EntityID = entityID;
            s_Data.QuadVertexBufferPtr++;
        }
        s_Data.QuadIndexCount+=6;
        s_Data.Stats.QuadCount++;

    }
    void Renderer2D::DrawSquare(const glm::vec3 &position, const glm::vec4 &color) {

    }
    void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityID){
        RC_PROFILE_FUNCTION();


//        if(s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices){
//            StartNewBatch();
//        }
//        for(size_t i =0; i< 4; i++){
//            s_Data.CircleVertexBufferPtr->WorldPosition = transform * s_Data.QuadVertexPositions[i];
//            s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.QuadVertexPositions[i] * 2.0f;
//            s_Data.CircleVertexBufferPtr->Color = color;
//            s_Data.CircleVertexBufferPtr->Thickness = thickness;
//            s_Data.CircleVertexBufferPtr->Thickness = fade;
//            s_Data.CircleVertexBufferPtr->EntityID = entityID;
//            s_Data.CircleVertexBufferPtr++;
//        }
//
//        s_Data.CircleIndexCount+=6;
//
//        s_Data.Stats.QuadCount++;
    }
    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref <Texture2D> &texture,
                              float tilingfactor,int entityID) {
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
            s_Data.QuadVertexBufferPtr->EntityID = entityID;
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

    void Renderer2D::BeginScene(const EditorCamera &camera) {
        RC_PROFILE_FUNCTION();

        glm::mat4 viewproj = camera.GetViewProjection();

        s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
       // s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DStorage::CameraData));

        s_Data.QuadShader->Bind();
        s_Data.QuadShader->
                SetMat4("u_ViewProjection",s_Data.CameraBuffer.ViewProjection);

//        s_Data.CircleShader->Bind();
//        s_Data.CircleShader->
//                SetMat4("u_ViewProjection",s_Data.CameraBuffer.ViewProjection);


        StartNewBatch();
    }

    void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID)
    {
        if(src.Texture)
            DrawQuad(transform,src.Color,src.Texture,src.TilingFactor,entityID);
        else
            DrawQuad(transform, src.Color, 1.0f,entityID);
    }



}