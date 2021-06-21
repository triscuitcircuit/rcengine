#version 410 core

out vec4 color;


layout(location =0) in vec3 a_Position;
layout(location =1) in vec4 a_Color;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Position;
out vec4 v_Color;

void main(void)
{
    v_Position = a_Position;
    v_Color= vec4(a_Position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);

    v_Color = a_Color;
    gl_Position = u_ViewProjection * u_Transform * vec4(v_Position,1.0);
}