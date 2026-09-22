#include "render_engine.h"
#include "mesh_renderer.h"

RenderEngine::RenderEngine() : 
        shader("shader.vert", "shader.frag"), 
        uiShader("shader_ui.vert", "shader_ui.frag"),
        debugForcesShader("shader_forces.vert", "shader_forces.frag") {
}

void RenderEngine::setMeshObjects(const std::vector<Object*> meshObjects) {
    this->meshObjects = meshObjects;

    // Generate VAOs for each mesh object
    for (auto object : meshObjects) {
        MeshRenderer* renderer = object->getComponent<MeshRenderer>();
        renderer->setup();

        if (!debugMode)
            continue;

        if (object->hasComponent<ColliderAABB>())
            colliderDebugRenderers.push_back(ColliderDebugRenderer());
    }
}
void RenderEngine::setLights(const std::vector<Object*> lights) {
    this->lights = lights;
}
void RenderEngine::setForces(const std::vector<Force*> forces) {
    this->forces = forces;

    if (!debugMode)
        return;

    // Create render object for each force for debugging
    for (auto force : forces) {
        forceDebugRenderers.push_back(ForceDebugRenderer());
    }
}

void RenderEngine::updateLights() {
    int numLights = lights.size();
    shader.setInt("numLights", numLights);

    for (int i = 0; i < numLights; ++i) {
        std::string index = "lights[" + std::to_string(i) + "]";

        Object* light = lights[i];
        LightSource* lightComponent = light->getComponent<LightSource>();

        shader.setVec3(index + ".position", light->getComponent<Transform>()->position);

        shader.setVec4(index + ".ambient", lightComponent->getAmbient());
        shader.setVec4(index + ".diffuse", lightComponent->getDiffuse());
        shader.setVec4(index + ".specular", lightComponent->getSpecular());

        shader.setFloat(index + ".kConstant", lightComponent->getKConstant());
        shader.setFloat(index + ".kLinear", lightComponent->getKLinear());
        shader.setFloat(index + ".KQuadratic", lightComponent->getKQuadratic());
    }
}

void RenderEngine::drawMeshObjects() {
    for (auto object : meshObjects) {
        MeshRenderer* renderer = object->getComponent<MeshRenderer>();
        renderer->draw(&shader);
    }
}

void RenderEngine::drawForces() {
    for (int i = 0; i < forces.size(); i++) {
        std::vector<glm::vec3> debugVector = forces[i]->getDebugVector();
        forceDebugRenderers[i].draw(debugVector);
    }
}

void RenderEngine::drawBoundingBoxes() {
    int renderId = 0;
    for (auto object : meshObjects) {
        if (!object->hasComponent<ColliderAABB>())
            continue;

        Transform* transform = object->getComponent<Transform>();
        ColliderAABB* collider = object->getComponent<ColliderAABB>();
        BoundingBox boundingBox = collider->getWorldBounds();
        
        colliderDebugRenderers[renderId].draw(boundingBox);

        renderId++;
    }
}

void RenderEngine::drawCursor() {
    cursor.draw();
}

void RenderEngine::render(Camera& camera) {
    // Background render
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Setup shader values for objects
    shader.use();
    shader.setMatrix4fv("view", camera.getViewMatrix());
    shader.setMatrix4fv("projection", camera.getProjectionMatrix());
    shader.setVec3("viewPos", (*camera.getComponent<Transform>()).position);
    updateLights();

    // Draw meshes
    drawMeshObjects();

    // Draw UI
    uiShader.use();
    drawCursor();

    if (!debugMode)
        return; // End early
        
    // Draw forces
    debugForcesShader.use();
    debugForcesShader.setMatrix4fv("view", camera.getViewMatrix());
    debugForcesShader.setMatrix4fv("projection", camera.getProjectionMatrix());
    drawForces();

    // Draw bounding boxes
    drawBoundingBoxes();
}