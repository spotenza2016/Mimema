#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 fragColor;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texture;
uniform mat4 matrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
out vec3 vFragPos;
out vec4 vFragColor;
out vec3 vFragNormal;
out vec2 vTexture;
void main()
{
   gl_Position = matrix * vec4(aPos.x, aPos.y, aPos.z, aPos.w);
   vFragPos = vec3(modelViewMatrix * vec4(aPos.x, aPos.y, aPos.z, aPos.w));
   vFragNormal = normalize(normalMatrix * normal);
   vFragColor = fragColor;
   vTexture = texture;
}
