#ifndef MESH_H
#define MESH_H

#pragma once

#include <vector>
#include <limits>
#include <glm/glm.hpp>
#include <iostream>

class Mesh {
public:
    Mesh();
    Mesh(std::vector<float> vertexData, std::vector<int> indices, float vertexSize);

    const std::vector<float>& getVertices() const;
    const std::vector<int>& getIndices() const;
    const float getVertexSize() const;

    glm::vec3 getMinBounds() const;
    glm::vec3 getMaxBounds() const;

private:
    std::vector<float> vertexData; // Pos, Tex, Normal
    std::vector<int> indices;

    float vertexSize;
};

#endif