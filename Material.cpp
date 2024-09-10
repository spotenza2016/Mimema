#include "Material.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

const std::string Mimema::Material::textureFolderLocation = "Textures/";

void Mimema::Material::setTexture(unsigned int& texture, const std::string& textureName) {
    glDeleteTextures(1, &texture);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load((textureFolderLocation + textureName).c_str(), &width, &height, &nrChannels, 0);

    if (data == nullptr) {
        std::cout << "Texture: " + textureName + " not found..." << std::endl;
        return;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    if (nrChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    else {
        std::cout << "Unknown material data produced..." << std::endl;
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

void Mimema::Material::assignDefaultTexture(unsigned int& texture) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    unsigned char data[4] = {255, 255, 255, 255};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

Mimema::Material::Material() {
    assignDefaultTexture(ambientColorTexture);
    assignDefaultTexture(diffuseColorTexture);
    assignDefaultTexture(specularColorTexture);
    assignDefaultTexture(specularExponentTexture);
    assignDefaultTexture(dissolveTexture);
}

Mimema::Material::~Material() {
    glDeleteTextures(1, &ambientColorTexture);
    glDeleteTextures(1, &diffuseColorTexture);
    glDeleteTextures(1, &specularColorTexture);
    glDeleteTextures(1, &specularExponentTexture);
    glDeleteTextures(1, &dissolveTexture);
}

void Mimema::Material::setAmbientColorTexture(const std::string& textureName) {
    ambientColorTextureName = textureName;
    setTexture(ambientColorTexture, textureName);
}

void Mimema::Material::setDiffuseColorTexture(const std::string& textureName) {
    diffuseColorTextureName = textureName;
    setTexture(diffuseColorTexture, textureName);
}

void Mimema::Material::setSpecularColorTexture(const std::string& textureName) {
    specularColorTextureName = textureName;
    setTexture(specularColorTexture, textureName);
}

void Mimema::Material::setSpecularExponentTexture(const std::string& textureName) {
    specularExponentTextureName = textureName;
    setTexture(specularExponentTexture, textureName);
}

void Mimema::Material::setDissolveTexture(const std::string& textureName) {
    dissolveTextureName = textureName;
    setTexture(dissolveTexture, textureName);
}

const std::string& Mimema::Material::getAmbientColorTextureName() const {
    return ambientColorTextureName;
}

unsigned int Mimema::Material::getAmbientColorTexture() const {
    return ambientColorTexture;
}

const std::string& Mimema::Material::getDiffuseColorTextureName() const {
    return diffuseColorTextureName;
}

unsigned int Mimema::Material::getDiffuseColorTexture() const {
    return diffuseColorTexture;
}

const std::string& Mimema::Material::getSpecularColorTextureName() const {
    return specularColorTextureName;
}

unsigned int Mimema::Material::getSpecularColorTexture() const {
    return specularColorTexture;
}

const std::string& Mimema::Material::getSpecularExponentTextureName() const {
    return specularExponentTextureName;
}

unsigned int Mimema::Material::getSpecularExponentTexture() const {
    return specularExponentTexture;
}

const std::string& Mimema::Material::getDissolveTextureName() const {
    return dissolveTextureName;
}

unsigned int Mimema::Material::getDissolveTexture() const {
    return dissolveTexture;
}