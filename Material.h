#ifndef MIMEMA_MATERIAL_H
#define MIMEMA_MATERIAL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/mat4x4.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

#include "tracy/Tracy.hpp"

namespace Mimema {
    class Material {
        static const std::string textureFolderLocation;

        unsigned int ambientColorTexture,
                     diffuseColorTexture,
                     specularColorTexture,
                     specularExponentTexture,
                     dissolveTexture;

        std::string ambientColorTextureName = "";
        std::string diffuseColorTextureName = "";
        std::string specularColorTextureName = "";
        std::string specularExponentTextureName = "";
        std::string dissolveTextureName = "";

        static void setTexture(unsigned int& texture, const std::string& textureName);

        void assignDefaultTexture(unsigned int& texture);

    public:
        glm::vec3 ambientColor = {0, 0, 0};
        glm::vec3 diffuseColor = {0, 0, 0};
        glm::vec3 specularColor = {0, 0, 0};
        float specularExponent = 0.0f;
        float dissolve = 1;
        int illuminationModel = 2;

        Material();

        Material(const Material& rhs) = delete;
        Material(const Material&& rhs) = delete;
        Material& operator=(const Material& rhs) = delete;
        Material& operator=(Material&&) = delete;

        ~Material();

        void setAmbientColorTexture(const std::string& textureName);
        void setDiffuseColorTexture(const std::string& textureName);
        void setSpecularColorTexture(const std::string& textureName);
        void setSpecularExponentTexture(const std::string& textureName);
        void setDissolveTexture(const std::string& textureName);

        const std::string& getAmbientColorTextureName() const;
        unsigned int getAmbientColorTexture() const;
        const std::string& getDiffuseColorTextureName() const;
        unsigned int getDiffuseColorTexture() const;
        const std::string& getSpecularColorTextureName() const;
        unsigned int getSpecularColorTexture() const;
        const std::string& getSpecularExponentTextureName() const;
        unsigned int getSpecularExponentTexture() const;
        const std::string& getDissolveTextureName() const;
        unsigned int getDissolveTexture() const;
    };
}

#endif // MIMEMA_MATERIAL_H