#version 330 core

layout (location = 0) in vec4 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexture;

out vec3 vFragPos;
out vec3 vFragNormal;
out vec2 vFragTexture;

uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

void main()
{
   gl_Position = mvpMatrix * vPos;
   vFragPos = vec3(modelMatrix * vPos);
   vFragNormal = normalMatrix * vNormal;
   vFragTexture = vTexture;
}
