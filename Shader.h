#ifndef MIMEMA_SHADER_H
#define MIMEMA_SHADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

#include "tracy/Tracy.hpp"
#include "tracy/TracyOpenGL.hpp"

#include "RenderState.h"

namespace Mimema {
    class Shader {
        static const std::string shaderFolderLocation;
        static const std::string vertexShaderFileExtension;
        static const std::string fragmentShaderFileExtension;

        static const int ambientColorTextureID = 0;
        static const int diffuseColorTextureID = 1;
        static const int specularColorTextureID = 2;
        static const int specularExponentTextureID = 3;
        static const int dissolveTextureID = 4;
        static const int maxDirectionalLights = 100;
        static const int maxPointLights = 100;

        unsigned int shaderID;

        static const char* mvpMatrixUniform;
        static const char* modelMatrixUniform;
        static const char* normalMatrixUniform;
        static const char* viewPosUniform;
        static const char* ambientLightIntensityUniform;
        static const char* ambientLightColorUniform;
        static const char* directionalLightCountUniform;
        static const char* directionalLightIntensitiesUniform;
        static const char* directionalLightColorsUniform;
        static const char* directionalLightDirectionsUniform;
        static const char* pointLightCountUniform;
        static const char* pointLightIntensitiesUniform;
        static const char* pointLightColorsUniform;
        static const char* pointLightPositionsUniform;
        static const char* pointLightAttenuationsUniform;
        static const char* ambientColorUniform;
        static const char* diffuseColorUniform;
        static const char* dissolveUniform;
        static const char* specularExponentUniform;
        static const char* specularColorUniform;
        static const char* illuminationModelUniform;
        static const char* ambientColorTextureUniform;
        static const char* diffuseColorTextureUniform;
        static const char* specularColorTextureUniform;
        static const char* specularExponentTextureUniform;
        static const char* dissolveTextureUniform;

        unsigned int mvpMatrixUID,
                        modelMatrixUID,
                        normalMatrixUID,
                        viewPosUID,
                        ambientLightIntensityUID,
                        ambientLightColorUID,
                        directionalLightCountUID,
                        directionalLightIntensitiesUID,
                        directionalLightColorsUID,
                        directionalLightDirectionsUID,
                        pointLightCountUID,
                        pointLightIntensitiesUID,
                        pointLightColorsUID,
                        pointLightPositionsUID,
                        pointLightAttenuationsUID,
                        ambientColorUID,
                        diffuseColorUID,
                        dissolveUID,
                        specularExponentUID,
                        specularColorUID,
                        illuminationModelUID,
                        ambientColorTextureUID,
                        diffuseColorTextureUID,
                        specularColorTextureUID,
                        specularExponentTextureUID,
                        dissolveTextureUID;

        std::string readShaderFile(const std::string& fileLocation);

    public:
        const std::string name;

        Shader(const std::string& fileName);

        Shader(const Shader& rhs) = delete;
        Shader(const Shader&& rhs) = delete;
        Shader& operator=(const Shader& rhs) = delete;
        Shader& operator=(Shader&&) = delete;

        ~Shader();

        void drawWithShader(const RenderState& renderState);
    };
}

#endif //MIMEMA_SHADER_H