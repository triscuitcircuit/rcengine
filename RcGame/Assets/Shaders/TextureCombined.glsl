#type vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out float v_TilingFactor;

void main()
{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
    v_TilingFactor = a_TilingFactor;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 410 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Textures[16];
//16 is compatible with more devices, but results in more draw calls

void main()
{
    //if statements used to avoid GLSL compiler error on some machines
    // ie no "int(float)" can be used
    // Thanks, Apple

    	if(v_TexIndex == 0.0){
            color = texture(u_Textures[0], v_TexCoord * v_TilingFactor) * v_Color;
        }
        if (v_TexIndex == 1.0) {
            color = texture(u_Textures[1], v_TexCoord * v_TilingFactor) * v_Color;
        }
        if (v_TexIndex == 2.0){
            color = texture(u_Textures[2], v_TexCoord * v_TilingFactor) * v_Color;
        }
        if (v_TexIndex == 3.0){
            color = texture(u_Textures[3], v_TexCoord * v_TilingFactor) * v_Color;
        }
        if (v_TexIndex == 4.0){
            color = texture(u_Textures[4], v_TexCoord * v_TilingFactor) * v_Color;
        }
        if (v_TexIndex == 5.0){
            color = texture(u_Textures[5], v_TexCoord * v_TilingFactor) * v_Color;
        }
        if (v_TexIndex == 6.0){
            color = texture(u_Textures[6], v_TexCoord * v_TilingFactor) * v_Color;
        }
        if (v_TexIndex == 7.0){
            color = texture(u_Textures[7], v_TexCoord * v_TilingFactor) * v_Color;
        }
        if (v_TexIndex == 8.0){
            color = texture(u_Textures[8], v_TexCoord * v_TilingFactor) * v_Color;
        }
        if (v_TexIndex == 9.0){
            color = texture(u_Textures[9], v_TexCoord * v_TilingFactor) * v_Color;
        }
        if (v_TexIndex == 10.0){
            color = texture(u_Textures[10], v_TexCoord * v_TilingFactor) * v_Color;
        }
        if (v_TexIndex == 11.0){
            color = texture(u_Textures[11], v_TexCoord * v_TilingFactor) * v_Color;
        }
        if (v_TexIndex == 12.0){
            color = texture(u_Textures[12], v_TexCoord * v_TilingFactor) * v_Color;
        }
        if (v_TexIndex == 13.0){
            color = texture(u_Textures[13], v_TexCoord * v_TilingFactor) * v_Color;
        }
        if (v_TexIndex == 14.0){
            color = texture(u_Textures[14], v_TexCoord * v_TilingFactor) * v_Color;
        }
        if (v_TexIndex == 15.0){
            color = texture(u_Textures[15], v_TexCoord * v_TilingFactor) * v_Color;
        }
    }