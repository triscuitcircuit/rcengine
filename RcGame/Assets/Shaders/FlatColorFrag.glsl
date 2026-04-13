#version 410 core

layout(location = 0) out vec4 color;

in vec3 v_Position;

uniform vec3 u_Color;
uniform sampler2D u_Sample;

void main()
{
    color = vec4(u_Color, 1.0);
}