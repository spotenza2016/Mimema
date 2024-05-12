#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 fragColor;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texture;
uniform mat4 matrix;
uniform vec3 lightVec;
out vec3 vFragPos;
out vec4 vFragColor;
out vec3 vFragNormal;
out vec3 vLightVec;
out vec2 vTexture;
void main()
{
   gl_Position = matrix * vec4(aPos.x, aPos.y, aPos.z, aPos.w);
   vFragPos = vec3(gl_Position);
   vFragNormal = normalize(vec3(matrix * vec4(normal, 0)));
   vLightVec = lightVec;
   vFragColor = fragColor;
   vTexture = texture;
}
