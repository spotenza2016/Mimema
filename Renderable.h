#ifndef MIMEMA_RENDERABLE_H
#define MIMEMA_RENDERABLE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>
#include <glm/ext.hpp>
#include <glm/geometric.hpp>
#include <vector>
#include <set>
#include <map>

#include "tracy/Tracy.hpp"

#include "Material.h"

namespace Mimema {
    class Renderable {
        static const std::string modelFolderLocation;
        static const std::string materialFolderLocation;

        struct TriangleBuffers {
            std::vector<glm::vec3> vertices;
            std::vector<glm::vec3> normals;
            std::vector<glm::vec2> textures;
        };
        struct TriangleGroup {
            std::vector<glm::vec<3, int>> vertexIndices;
            std::vector<glm::vec<3, int>> normalIndices;
            std::vector<glm::vec<3, int>> textureIndices;
        };

        // Buffer Constants
        static const int vboBlockSize = 9;
        static const int vboVertexBlockSize = 4;
        static const int vboNormalBlockSize = 3;
        static const int vboTextureBlockSize = 2;

        static const glm::vec3 defaultNormal;
        static const glm::vec2 defaultTextureCoord;

        unsigned int VAO, VBO;
        std::vector<std::pair<Material*, int>> triangles;

        glm::vec3 bbPos;
        glm::vec3 bbSize;

        void readMtlFile(const std::string& fileLocation, std::map<std::string, Material*>& materials);

        void setBuffers(const TriangleBuffers& data,
                        const std::map<std::string, TriangleGroup*>& triangleGroups);

    public:
        // Generates a renderable based on an obj file
        Renderable(const std::string& fileLocation);

        // Generates a cubic renderable object
        Renderable(const glm::vec3& position, const glm::vec3& size);

        Renderable(const Renderable& rhs) = delete;
        Renderable(const Renderable&& rhs) = delete;
        Renderable& operator=(const Renderable& rhs) = delete;
        Renderable& operator=(Renderable&&) = delete;

        ~Renderable();

        // Getters
        unsigned int getVAO() const;
        unsigned int getVBO() const;
        const glm::vec3& getBBPos() const;
        const glm::vec3& getBBSize() const;
        const std::vector<std::pair<Material*, int>>& getTriangles() const;
    };
}

#endif //MIMEMA_RENDERABLE_H