#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/geometric.hpp>

#include "component.h"

class Transform : public Component {

    public:
        Transform();
        Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

        void translate(glm::vec3 position);
        void setPosition(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

        glm::mat4 getModel() const;

        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
};

#endif