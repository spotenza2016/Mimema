#version 330 core
in vec3 vFragPos;
in vec4 vFragColor;
in vec3 vFragNormal;
in vec3 vLightVec;
in vec2 vTexture;
out vec4 FragColor;
uniform float ambientLightIntensity;
uniform float lightIntensity;
uniform float phongExponent;
uniform vec3 specularColor;
uniform sampler2D textureFile;
void main()
{
   // Shading
   vec4 texturedColor = vFragColor * texture(textureFile, vTexture);
   vec3 ambientLight = ambientLightIntensity * vec3(texturedColor);
   vec3 diffuseLight = lightIntensity * max(0, dot(vFragNormal, -1 * vLightVec)) * vec3(texturedColor);
   vec3 eyeVec = vec3(0, 0, -1);
   eyeVec = vec3(normalize(-1 * vFragPos));
   vec3 h = normalize(eyeVec + -1 * vLightVec);
   vec3 specularLight = lightIntensity * max(0, pow(dot(vFragNormal, h), phongExponent)) * specularColor;
   vec4 newColor = vec4(ambientLight + diffuseLight + specularLight, texturedColor[3]);

   newColor[0] = min(1, newColor[0]);
   newColor[1] = min(1, newColor[1]);
   newColor[2] = min(1, newColor[2]);

   FragColor = newColor;
}
