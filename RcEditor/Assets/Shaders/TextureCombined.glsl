#type vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int a_EntityID;


uniform mat4 u_ViewProjection;
//layout(std140, binding = 0) uniform Camera
//{
//    mat4 u_ViewProjection;
//};

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

#type fragment
#version 410 core

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

struct VertexOutput{
    vec4 Color;
    vec2 TexCoord;
    float TexIndex;
    float TilingFactor;
};

layout(location = 0) in VertexOutput Input;
layout(location = 4) flat in int v_EntityID;


uniform sampler2D u_Textures[16];
//16 is compatible with more devices, but results in more draw calls

void main()
{
    //if statements used to avoid GLSL compiler error on some machines
    // ie no "int(float)" can be used
    // Thanks, Apple
        vec4 texColor = Input.Color; 
    	if(Input.TexIndex == 0.0){
            texColor *= texture(u_Textures[0], Input.TexCoord * Input.TilingFactor) * Input.Color;
        }
        if (Input.TexIndex == 1.0) {
            texColor *= texture(u_Textures[1], Input.TexCoord * Input.TilingFactor) * Input.Color;
        }
        if (Input.TexIndex == 2.0){
            texColor *= texture(u_Textures[2], Input.TexCoord * Input.TilingFactor) * Input.Color;
        }
        if (Input.TexIndex == 3.0){
            texColor *= texture(u_Textures[3], Input.TexCoord * Input.TilingFactor) * Input.Color;
        }
        if (Input.TexIndex == 4.0){
            texColor *= texture(u_Textures[4], Input.TexCoord * Input.TilingFactor) * Input.Color;
        }
        if (Input.TexIndex == 5.0){
            texColor *= texture(u_Textures[5], Input.TexCoord * Input.TilingFactor) * Input.Color;
        }
        if (Input.TexIndex == 6.0){
            texColor *= texture(u_Textures[6], Input.TexCoord * Input.TilingFactor) * Input.Color;
        }
        if (Input.TexIndex == 7.0){
            texColor *= texture(u_Textures[7], Input.TexCoord * Input.TilingFactor) * Input.Color;
        }
        if (Input.TexIndex == 8.0){
            texColor *= texture(u_Textures[8], Input.TexCoord * Input.TilingFactor) * Input.Color;
        }
        if (Input.TexIndex == 9.0){
            texColor *= texture(u_Textures[9], Input.TexCoord * Input.TilingFactor) * Input.Color;
        }
        if (Input.TexIndex == 10.0){
            texColor *= texture(u_Textures[10], Input.TexCoord * Input.TilingFactor) * Input.Color;
        }
        if (Input.TexIndex == 11.0){
            texColor *= texture(u_Textures[11], Input.TexCoord * Input.TilingFactor) * Input.Color;
        }
        if (Input.TexIndex == 12.0){
            texColor *= texture(u_Textures[12], Input.TexCoord * Input.TilingFactor) * Input.Color;
        }
        if (Input.TexIndex == 13.0){
            texColor *= texture(u_Textures[13], Input.TexCoord * Input.TilingFactor) * Input.Color;
        }
        if (Input.TexIndex == 14.0){
            texColor *= texture(u_Textures[14], Input.TexCoord * Input.TilingFactor) * Input.Color;
        }
        if (Input.TexIndex == 15.0){
            texColor *= texture(u_Textures[15], Input.TexCoord * Input.TilingFactor) * Input.Color;
        }
        color = texColor;
        color2 = 50;
    }