#pragma once
#include <list>
#include "Force/ForceRegistry.h"
#include "Force/GravityForceGenerator.h"


namespace physics {
	class PhysicsParticle;

	class PhysicsWorld
	{
	public:
		//The list of ALL our particles
		std::list<PhysicsParticle*> Particles;

		//Function to add particles to the list
		void AddParticle(PhysicsParticle* toAdd);

		//Universal update function to call the updates of All
		void Update(float time);

		ForceRegistry forceRegistry;

	private:
		//Updates the particle list
		void UpdateParticleList();

		GravityForceGenerator Gravity = GravityForceGenerator(MyVector(0, -9.8f, 0));
	};
}


