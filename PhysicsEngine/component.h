#ifndef COMPONENT_H
#define COMPONENT_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

class Object;

class Component {
    friend class Object;

public:
    virtual ~Component() = default;

    Object* getObject() const {
        return object;
    }

protected:
    Object* object = nullptr;
};

#endif