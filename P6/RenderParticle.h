#pragma once
#include "PhysicsWorld.h"
#include "../GameObject.h"
using namespace physics;

class RenderParticle
{
public:
	physics::PhysicsParticle* PhysicsParticle;

	GameObject* RenderObject;

	glm::vec4 Color;

	RenderParticle(physics::PhysicsParticle* p, GameObject* obj)
		: PhysicsParticle(p), RenderObject(obj), Color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)) {
		// Additional initialization code if needed
	}

	RenderParticle(physics::PhysicsParticle* p, GameObject* obj, glm::vec4 c) : PhysicsParticle(p), RenderObject(obj), Color(c) {};

	void Draw(glm::mat4 identity_matrix, glm::mat4 projection_matrix, glm::mat4 view_matrix);
};

