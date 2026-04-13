#type vertex
#version 410 core

layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in vec3 a_LocalPosition;
layout(location = 2) in vec4 a_Color;
layout(location = 5) in int a_EntityID;

uniform mat4 u_ViewProjection;
uniform mat4 u_mv_Matrix;

struct VertexOutput{
    vec3 LocalPosition;
    vec4 Color;
};

layout (location = 0) out VertexOutput Input;
layout (location = 2) flat out int v_EntityID;

void main(void) {
    Input.LocalPosition = a_LocalPosition;
    Input.Color = a_Color;

    gl_Position = u_ViewProjection * u_mv_Matrix * vec4(a_WorldPosition,1.0);
}

#type fragment
#version 410 core

layout(location = 0) out vec4 o_Color;
layout(location = 2) out int o_EntityID;

struct VertexOutput{
    vec3 LocalPosition;
    vec4 Color;
};

layout(location = 0) in VertexOutput Input;
layout(location = 2) flat in int v_EntityID;

void main(void){
    o_Color = Input.Color;
    o_EntityID = v_EntityID;
}