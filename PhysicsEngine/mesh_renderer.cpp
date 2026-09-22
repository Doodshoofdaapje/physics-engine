#include "mesh_renderer.h"
#include "object.h"

MeshRenderer::MeshRenderer(const char* objectPath) {
    // Initialize mesh data
    ModelFileParser* parser = parserFactory(objectPath);
    parser->parse(objectPath);

    this->textureSize = parser->getTextureSize();
    float vertexSize = 6 + textureSize;

    this->mesh = Mesh(parser->getVertexData(), parser->getIndices(), vertexSize);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    // Initialize texture data
    this->textureID;
    this->textured = false;

    // Initialize material data
    this->material.shininess = 1;
    this->material.ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    this->material.diffuse = glm::vec4(1.0f);
    this->material.specular = glm::vec4(1.0f);

    delete parser;
}

ModelFileParser* MeshRenderer::parserFactory(const char* objectPath) {
    // File management
    std::string path(objectPath);
    std::string fileExtention = path.substr(path.find_last_of("."));

    // File parser factory
    if (fileExtention == ".obj") {
        return new ObjModelParser();
    }
    else {
        std::cout << "ERROR::OBJECT::WRONG_FILE_FORMAT" << std::endl;
        return nullptr;
    }
}

void MeshRenderer::setup() {
    // Bind to array
    glBindVertexArray(VAO);

    // Setup texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Object data
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.getVertices().size() * sizeof(float), mesh.getVertices().data(), GL_STATIC_DRAW);

    // EBO for efficient memory
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndices().size() * sizeof(int), mesh.getIndices().data(), GL_STATIC_DRAW);

    // Specify position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mesh.getVertexSize() * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Specify texture attribute
    glVertexAttribPointer(1, textureSize, GL_FLOAT, GL_FALSE, mesh.getVertexSize() * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Specify vertex normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, mesh.getVertexSize() * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind buffer and array
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshRenderer::draw(Shader* shader) {
    Transform* transform = getObject()->getComponent<Transform>();
    glm::mat4 model = glm::mat4(1.0f);

    // Transform Geometry view to World view
    shader->setBool("textured", textured);
    shader->setMatrix4fv("model", transform->getModel());
    shader->setFloat("material.shininess", material.shininess);
    shader->setVec4("material.ambient", material.ambient);
    shader->setVec4("material.diffuse", material.diffuse);
    shader->setVec4("material.specular", material.specular);

    shader->use();

    if (textured && textureID) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, mesh.getIndices().size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

MeshRenderer* MeshRenderer::setTexture(bool isTextured, const char* texturePath) {
    if (textureID) {
        glDeleteTextures(1, &textureID);
    }

    this->textured = isTextured;
    this->textureID = createTexture(texturePath);
    return this;
}

MeshRenderer* MeshRenderer::setMaterial(float shininess, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular) {
    this->material.shininess = shininess;
    this->material.ambient = ambient;
    this->material.diffuse = diffuse;
    this->material.specular = specular;
    return this;
}

int MeshRenderer::createTexture(const char* texturePath) {
    if (!textured) {
        return 0;
    }

    // Setup and bind texture object
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set the texture wrapping/filtering options on the currently bound texture object
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "Generated texture" << std::endl;
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return texture;
}

const Mesh& MeshRenderer::getMesh() {
    return this->mesh;
}