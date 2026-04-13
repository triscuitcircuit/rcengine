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

struct Camera
{
    float4x4 u_ViewProjection;
};

struct main0_out
{
    float4 VertexOutput_Color [[user(locn0)]];
    float2 VertexOutput_TexCoord [[user(locn1)]];
    float VertexOutput_TexIndex [[user(locn2)]];
    float VertexOutput_TilingFactor [[user(locn3)]];
    int v_EntityID [[user(locn4)]];
    float4 gl_Position [[position]];
};

struct main0_in
{
    float3 a_Position [[attribute(0)]];
    float4 a_Color [[attribute(1)]];
    float2 a_TexCoord [[attribute(2)]];
    float a_TexIndex [[attribute(3)]];
    float a_TilingFactor [[attribute(4)]];
    int a_EntityID [[attribute(5)]];
};

vertex main0_out main0(main0_in in [[stage_in]], constant Camera& _49 [[buffer(0)]])
{
    main0_out out = {};
    VertexOutput Input = {};
    Input.Color = in.a_Color;
    Input.TexCoord = in.a_TexCoord;
    Input.TexIndex = in.a_TexIndex;
    Input.TilingFactor = in.a_TilingFactor;
    out.v_EntityID = in.a_EntityID;
    out.gl_Position = _49.u_ViewProjection * float4(in.a_Position, 1.0);
    out.VertexOutput_Color = Input.Color;
    out.VertexOutput_TexCoord = Input.TexCoord;
    out.VertexOutput_TexIndex = Input.TexIndex;
    out.VertexOutput_TilingFactor = Input.TilingFactor;
    return out;
}

