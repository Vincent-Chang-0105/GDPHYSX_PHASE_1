#pragma once
#include "MyVector.h"

namespace physics{

	class PhysicsParticle : public MyVector
	{
		protected:
			bool isDestroyed = false;

			MyVector accumulatedForce = MyVector(0, 0, 0);
		public:
			float mass = 0;
			MyVector Velocity;
			MyVector Position;
			MyVector Acceleration;

			float damping = 0.9f;

			float lifespan;

		protected:
			void UpdatePosition(float time);
			void UpdateVelocity(float time);

		public:
			PhysicsParticle();
			void update(float time);

			//Forces
			void AddForce(MyVector force);

			void ResetForce();

			//life
			void AddLifeSpan();
			void UpdateLifeSpan(float time);

		public:
			void Destroy();
			bool IsDestroyed() { return isDestroyed; }

	};
}

