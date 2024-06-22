#include "PhysicsWorld.h"
#include "PhysicsParticle.h"

using namespace physics;

void PhysicsWorld::AddParticle(PhysicsParticle* toAdd)
{
	Particles.push_back(toAdd);

	forceRegistry.Add(toAdd, &Gravity);
}

void PhysicsWorld::Update(float time)
{
	UpdateParticleList();

	forceRegistry.UpdateForces(time);

	for (std::list<PhysicsParticle*>::iterator p = Particles.begin(); p != Particles.end(); p++) {
		(*p)->update(time);
	}
}

void PhysicsWorld::UpdateParticleList()
{
	Particles.remove_if([](physics::PhysicsParticle* p) {
		return p->IsDestroyed();
		}
	);
}
