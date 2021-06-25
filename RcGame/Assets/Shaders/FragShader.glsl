#version 410 core

layout(location=0) out vec4 color;

in vec4 v_Color;

void main(void)
{
    color = v_Color;

}