#pragma once

using namespace Moon;

#include "Minecraft/World/World.h"


namespace Minecraft {

	struct RayWorldHitResult
	{
		bool Hit = false;
		float HitFraction = 0.0f;
		Int3 HitBlock;
	};

	class Ray
	{
	public:
		glm::vec3 Origin, Direction;
		float Distance;

		Ray(const glm::vec3& origin, const glm::vec3& direction, float distance)
			: Origin(origin), Direction(direction), Distance(distance) {}

		/**
		 *
		 * HitPosition = rayOrigin + rayDirection * rayDistance * returnValue
		 *
		 * @return Fraction along ray that hits the world.
		 */
		float WorldIntersection(const Ref<World>& world, RayWorldHitResult& outResult);

	};

}
