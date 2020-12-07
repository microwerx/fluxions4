#version 450 core

layout(push_constant) uniform FragmentPushConstants {
    layout(offset = 64)
    vec4 Kd;
    vec4 Ks;
    vec4 Roughness;
    vec4 mixes;
} Material;

layout(std140, set = 0, binding = 1) uniform FragmentUniformBlock {
    mat4 modelMatrix;
    mat4 viewMatrix;
    mat4 modelviewMatrix;
    mat4 modelviewprojectionMatrix;
    mat3 normalMatrix;
    vec4 color;
};

layout(set = 0, binding = 2) uniform sampler2D map_Kd;

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec4 vColor;
layout(location = 3) in vec3 vTexCoord;

layout(location = 0) out vec4 oColor;

vec4 lightSource = vec4(2.0, 2.0, 20.0, 0.0);

void main()
{
    vec3 N = normalize(vNormal);
    vec3 L = normalize(lightSource.xyz - vPosition);
    float NdotL = max(0.0, dot(N, L));
    vec3 texel = vec3(1.0);//texture(tex0, vTexCoord.st).rgb;

    oColor = vec4(NdotL * 0.5 * (texel + vColor.rgb), 1.0);
}
