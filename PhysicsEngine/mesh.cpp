#include "mesh.h"

Mesh::Mesh() {

}

Mesh::Mesh(std::vector<float> vertexData, std::vector<int> indices, float vertexSize) {
	this->vertexData = vertexData;
	this->indices = indices;
	this->vertexSize = vertexSize;
}

const std::vector<float>& Mesh::getVertices() const {
	return this->vertexData;
}

const std::vector<int>& Mesh::getIndices() const {
	return this->indices;
}

const float Mesh::getVertexSize() const {
	return this->vertexSize;
}

glm::vec3 Mesh::getMinBounds() const {
	float minX = std::numeric_limits<float>::max();;
	float minY = std::numeric_limits<float>::max();;
	float minZ = std::numeric_limits<float>::max();;

	for (int i = 0; i + 2 < vertexData.size(); i += vertexSize) {
		minX = std::min(minX, vertexData.at(i));
		minY = std::min(minY, vertexData.at(i+1));
		minZ = std::min(minZ, vertexData.at(i+2));
	}

	std::cout << "BB " << minX << minY << minZ << std::endl;

	return { minX, minY, minZ };
}

glm::vec3 Mesh::getMaxBounds() const {
	float maxX = std::numeric_limits<float>::lowest();
	float maxY = std::numeric_limits<float>::lowest();
	float maxZ = std::numeric_limits<float>::lowest();

	for (int i = 0; i + 2 < vertexData.size(); i += vertexSize) {
		maxX = std::max(maxX, vertexData.at(i));
		maxY = std::max(maxY, vertexData.at(i + 1));
		maxZ = std::max(maxZ, vertexData.at(i + 2));
	}

	std::cout << "BB " << maxX << maxY << maxZ << std::endl;

	return { maxX, maxY, maxZ };
}