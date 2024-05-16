#version 330 core
in vec3 vFragPos;
in vec4 vFragColor;
in vec3 vFragNormal;
in vec2 vTexture;
out vec4 FragColor;
uniform float ambientLightIntensity;
uniform float lightIntensity;
uniform float phongExponent;
uniform vec3 specularColor;
uniform vec3 lightVec;
uniform sampler2D textureFile;
void main()
{
   // Shading
   vec4 texturedColor = vFragColor * texture(textureFile, vTexture);
   vec3 ambientLight = ambientLightIntensity * vec3(texturedColor);
   vec3 diffuseLight = lightIntensity * max(0, dot(vFragNormal, -1 * lightVec)) * vec3(texturedColor);
   vec3 eyeVec = normalize(-1 * vFragPos);
   vec3 r = normalize(reflect(normalize(lightVec), vFragNormal));
   vec3 specularLight = lightIntensity * max(0, pow(max(dot(r, eyeVec), 0), phongExponent)) * specularColor;
   vec4 newColor = vec4(ambientLight + diffuseLight + specularLight, texturedColor[3]);

   newColor[0] = min(1, newColor[0]);
   newColor[1] = min(1, newColor[1]);
   newColor[2] = min(1, newColor[2]);

   FragColor = newColor;
}
