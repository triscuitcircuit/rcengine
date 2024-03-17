#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int a_EntityID;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjection;
};

struct VertexOutput
{
    vec4 Color;
    vec2 TexCoord;
    float TexIndex;
    float TilingFactor;
};

layout (location = 0) out VertexOutput Input;
layout (location = 4) flat out int v_EntityID;

void main()
{

    Input.Color = a_Color;
    Input.TexCoord = a_TexCoord;
    Input.TexIndex = a_TexIndex;
    Input.TilingFactor = a_TilingFactor;
    v_EntityID = a_EntityID;

    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}
