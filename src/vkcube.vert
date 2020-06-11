#version 420 core

layout(std140, set = 0, binding = 0) uniform block {
    uniform mat4 modelviewMatrix;
    uniform mat4 modelviewprojectionMatrix;
    uniform mat3 normalMatrix;
    uniform vec4 color;
};

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec4 aColor;
layout(location = 3) in vec3 aTexCoord;

layout(location = 0) out vec3 vPosition;
layout(location = 1) out vec3 vNormal;
layout(location = 2) out vec4 vColor;
layout(location = 3) out vec3 vTexCoord;

void main()
{
    gl_Position = modelviewprojectionMatrix * aPosition;
    vec4 vPosition4 = modelviewMatrix * aPosition;
    vPosition = vPosition4.xyz / vPosition4.w;
    vNormal = normalMatrix * aNormal;
    vColor = aColor;
    vTexCoord = aTexCoord;
}
