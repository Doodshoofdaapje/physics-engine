#pragma once

#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>
#include <array>

#include "solver.h"
#include "object.h"
#include "gravity.h"
#include "light_source.h"
#include "camera.h"
#include "force_debug_renderer.h"
#include "collider_debug_renderer.h"
#include "cursor.h"
#include "collider_aabb.h"

class RenderEngine {
	private:
		bool debugMode = true; // Shows optional debug visualization

		Shader shader; // Main Shader
		Shader uiShader; // Shader for UI
		Shader debugForcesShader; // Shader for debugging forces

		// Optional pointers to objecs to render
		std::vector<Object*> meshObjects;
		std::vector<Object*> lights;
		std::vector<Force*> forces;

		// Helper objects to save rendering states
		std::vector<ForceDebugRenderer> forceDebugRenderers; // Index of debugRenderer must match index of force
		std::vector<ColliderDebugRenderer> colliderDebugRenderers; // Index of debugRenderer must match order of objects

		// UI elements
		Cursor cursor;

		// Draw functions
		void updateLights();
		void drawMeshObjects();
		void drawForces();
		void drawBoundingBoxes();
		void drawCursor();

	public:
		// Constructor and disable copying
		RenderEngine();
		RenderEngine(const RenderEngine&) = delete;
		RenderEngine& operator=(const RenderEngine&) = delete;
		
		// Setters for object references
		void setMeshObjects(const std::vector<Object*> meshObjects);
		void setLights(const std::vector<Object*> lights);
		void setForces(const std::vector<Force*> forces);

		// Main render loop
		void render(Camera& camera);

};

#endif