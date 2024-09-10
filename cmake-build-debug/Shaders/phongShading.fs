#version 330 core

in vec3 vFragPos;
in vec3 vFragNormal;
in vec2 vFragTexture;

out vec4 FragColor;

uniform vec3 viewPos;
uniform float ambientLightIntensity;
uniform vec3 ambientLightColor;
uniform int directionalLightCount;
uniform float[100] directionalLightIntensities;
uniform vec3[100] directionalLightColors;
uniform vec3[100] directionalLightDirections;
uniform int pointLightCount;
uniform float[100] pointLightIntensities;
uniform vec3[100] pointLightColors;
uniform vec3[100] pointLightPositions;
uniform vec3[100] pointLightAttenuations;
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform float dissolve;
uniform float specularExponent;
uniform vec3 specularColor;
uniform int illuminationModel;
uniform sampler2D ambientColorTexture;
uniform sampler2D diffuseColorTexture;
uniform sampler2D specularColorTexture;
uniform sampler2D specularExponentTexture;
uniform sampler2D dissolveTexture;

vec3 applyPhongLighting(vec3 direction, vec3 lightColor)
{
   vec3 totalColor = vec3(0, 0, 0);

   vec3 diffuseLight = lightColor * max(0, dot(vFragNormal, direction)) * diffuseColor * vec3(texture(diffuseColorTexture, vFragTexture));
   totalColor += diffuseLight;

   if (illuminationModel >= 2) {
      vec3 eyeVec = normalize(viewPos - vFragPos);
      vec3 r = normalize(reflect(-1 * direction, vFragNormal));
      float specularExponentValue = specularExponent * texture(specularColorTexture, vFragTexture).a;
      vec3 specularLight = lightColor * max(0, pow(max(dot(r, eyeVec), 0), specularExponent)) * specularColor * vec3(texture(specularColorTexture, vFragTexture));
      totalColor += specularLight;
  }

  return totalColor;
}

void main()
{
   vec3 totalColor = vec3(0, 0, 0);

   // Ambient Light
   if (illuminationModel >= 1) {
      totalColor += ambientLightIntensity * ambientLightColor * ambientColor * vec3(texture(ambientColorTexture, vFragTexture));
   }

   for (int i = 0; i < pointLightCount; i++) {
       vec3 direction = normalize(pointLightPositions[i] - vFragPos);

       float c1 = pointLightAttenuations[i][0];
       float c2 = pointLightAttenuations[i][1];
       float c3 = pointLightAttenuations[i][2];
       float d = length(pointLightPositions[i] - vFragPos);
       float attenuation = 1.0 / (c1 + c2*d + c3*d);

       vec3 lightColor = attenuation * pointLightIntensities[i] * pointLightColors[i];

       totalColor += applyPhongLighting(direction, lightColor);
   }

   for (int i = 0; i < directionalLightCount; i++) {
       vec3 direction = normalize(-1 * directionalLightDirections[i]);
       vec3 lightColor = directionalLightIntensities[i] * directionalLightColors[i];

       totalColor += applyPhongLighting(direction, lightColor);
   }

   totalColor[0] = min(1, totalColor[0]);
   totalColor[1] = min(1, totalColor[1]);
   totalColor[2] = min(1, totalColor[2]);

   float dissolveValue = dissolve * texture(dissolveTexture, vFragTexture).a;

   FragColor = vec4(totalColor, dissolveValue);
}
