#include "Material.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const glm::vec3& Material::getKd() {
    return kd;
}

const string &Material::getMap_kd() {
    return map_kd;
}

unsigned int Material::getTexture() {
    return texture;
}

void Material::setKd(const glm::vec3& kd) {
    this->kd = kd;
}

void Material::setMap_kd(const string& map_kd) {
    this->map_kd = map_kd;

    glDeleteTextures(1, &texture);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    testVal = true;
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    // todo use MTL path
    unsigned char* data = stbi_load(map_kd.c_str(), &width, &height, &nrChannels, 0);
    if (data == nullptr) {
        cout << "Texture: " + map_kd + " not found..." << endl;
        return;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    if (nrChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    else {
        cout << "Unknown material data produced..." << endl;
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

Material::Material() {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    unsigned char data[3] = {255, 255, 255};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    testVal = true;
}

Material::~Material() {
    glDeleteTextures(1, &texture);
}