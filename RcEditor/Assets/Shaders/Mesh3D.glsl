#type vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
uniform mat4 u_NormalMatrix;  // transpose(inverse(mat3(u_Model))), pre-computed on CPU

struct VertexOutput {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
};

layout(location = 0) out VertexOutput vs_Out;
layout(location = 3) flat out int v_EntityID;

uniform int u_EntityID;

void main() {
    vec4 worldPos    = u_Model * vec4(a_Position, 1.0);
    vs_Out.FragPos   = worldPos.xyz;
    vs_Out.Normal    = mat3(u_NormalMatrix) * a_Normal;
    vs_Out.TexCoords = a_TexCoords;
    v_EntityID       = u_EntityID;
    gl_Position      = u_ViewProjection * worldPos;
}

#type fragment
#version 410 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int  o_EntityID;

struct VertexOutput {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
};

layout(location = 0) in VertexOutput vs_Out;
layout(location = 3) flat in int v_EntityID;

uniform vec4      u_Color;
uniform vec3      u_LightDir;    // direction light travels (world space)
uniform vec3      u_LightColor;
uniform vec3      u_ViewPos;
uniform int       u_UseTexture;  // 0 = color only, 1 = texture
uniform sampler2D u_Texture;

void main() {
    vec3 norm     = normalize(vs_Out.Normal);
    vec3 lightDir = normalize(-u_LightDir);

    // Ambient
    vec3 ambient = 0.15 * u_LightColor;

    // Diffuse
    float diff   = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor;

    // Specular (Blinn-Phong)
    vec3 viewDir    = normalize(u_ViewPos - vs_Out.FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec      = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
    vec3 specular   = 0.4 * spec * u_LightColor;

    vec4 baseColor = u_Color;
    if (u_UseTexture == 1) {
        baseColor *= texture(u_Texture, vs_Out.TexCoords);
    }

    vec3 lighting = (ambient + diffuse + specular) * baseColor.rgb;
    o_Color    = vec4(lighting, baseColor.a);
    o_EntityID = v_EntityID;
}
