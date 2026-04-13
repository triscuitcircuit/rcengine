#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct VertexOutput
{
    float4 Color;
    float2 TexCoord;
    float TexIndex;
    float TilingFactor;
};

struct main0_out
{
    float4 color [[color(0)]];
    int color2 [[color(1)]];
};

struct main0_in
{
    float4 VertexOutput_Color [[user(locn0)]];
    float2 VertexOutput_TexCoord [[user(locn1)]];
    float VertexOutput_TexIndex [[user(locn2)]];
    float VertexOutput_TilingFactor [[user(locn3)]];
    int v_EntityID [[user(locn4)]];
};

fragment main0_out main0(main0_in in [[stage_in]], array<texture2d<float>, 16> u_Textures [[texture(0)]], array<sampler, 16> u_TexturesSmplr [[sampler(0)]])
{
    main0_out out = {};
    VertexOutput Input = {};
    Input.Color = in.VertexOutput_Color;
    Input.TexCoord = in.VertexOutput_TexCoord;
    Input.TexIndex = in.VertexOutput_TexIndex;
    Input.TilingFactor = in.VertexOutput_TilingFactor;
    float4 texColor = Input.Color;
    if (Input.TexIndex == 0.0)
    {
        texColor *= (u_Textures[0].sample(u_TexturesSmplr[0], (Input.TexCoord * Input.TilingFactor)) * Input.Color);
    }
    if (Input.TexIndex == 1.0)
    {
        texColor *= (u_Textures[1].sample(u_TexturesSmplr[1], (Input.TexCoord * Input.TilingFactor)) * Input.Color);
    }
    if (Input.TexIndex == 2.0)
    {
        texColor *= (u_Textures[2].sample(u_TexturesSmplr[2], (Input.TexCoord * Input.TilingFactor)) * Input.Color);
    }
    if (Input.TexIndex == 3.0)
    {
        texColor *= (u_Textures[3].sample(u_TexturesSmplr[3], (Input.TexCoord * Input.TilingFactor)) * Input.Color);
    }
    if (Input.TexIndex == 4.0)
    {
        texColor *= (u_Textures[4].sample(u_TexturesSmplr[4], (Input.TexCoord * Input.TilingFactor)) * Input.Color);
    }
    if (Input.TexIndex == 5.0)
    {
        texColor *= (u_Textures[5].sample(u_TexturesSmplr[5], (Input.TexCoord * Input.TilingFactor)) * Input.Color);
    }
    if (Input.TexIndex == 6.0)
    {
        texColor *= (u_Textures[6].sample(u_TexturesSmplr[6], (Input.TexCoord * Input.TilingFactor)) * Input.Color);
    }
    if (Input.TexIndex == 7.0)
    {
        texColor *= (u_Textures[7].sample(u_TexturesSmplr[7], (Input.TexCoord * Input.TilingFactor)) * Input.Color);
    }
    if (Input.TexIndex == 8.0)
    {
        texColor *= (u_Textures[8].sample(u_TexturesSmplr[8], (Input.TexCoord * Input.TilingFactor)) * Input.Color);
    }
    if (Input.TexIndex == 9.0)
    {
        texColor *= (u_Textures[9].sample(u_TexturesSmplr[9], (Input.TexCoord * Input.TilingFactor)) * Input.Color);
    }
    if (Input.TexIndex == 10.0)
    {
        texColor *= (u_Textures[10].sample(u_TexturesSmplr[10], (Input.TexCoord * Input.TilingFactor)) * Input.Color);
    }
    if (Input.TexIndex == 11.0)
    {
        texColor *= (u_Textures[11].sample(u_TexturesSmplr[11], (Input.TexCoord * Input.TilingFactor)) * Input.Color);
    }
    if (Input.TexIndex == 12.0)
    {
        texColor *= (u_Textures[12].sample(u_TexturesSmplr[12], (Input.TexCoord * Input.TilingFactor)) * Input.Color);
    }
    if (Input.TexIndex == 13.0)
    {
        texColor *= (u_Textures[13].sample(u_TexturesSmplr[13], (Input.TexCoord * Input.TilingFactor)) * Input.Color);
    }
    if (Input.TexIndex == 14.0)
    {
        texColor *= (u_Textures[14].sample(u_TexturesSmplr[14], (Input.TexCoord * Input.TilingFactor)) * Input.Color);
    }
    if (Input.TexIndex == 15.0)
    {
        texColor *= (u_Textures[15].sample(u_TexturesSmplr[15], (Input.TexCoord * Input.TilingFactor)) * Input.Color);
    }
    out.color = texColor;
    out.color2 = in.v_EntityID;
    return out;
}

