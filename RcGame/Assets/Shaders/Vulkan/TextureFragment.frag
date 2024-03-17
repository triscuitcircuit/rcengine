#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

struct VertexOutput{
    vec4 Color;
    vec2 TexCoord;
    float TexIndex;
    float TilingFactor;
};

layout(location = 0) in VertexOutput Input;
layout(location = 4) in flat int v_EntityID;

layout (binding =0) uniform sampler2D u_Textures[16];
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
    color2 = v_EntityID;
}