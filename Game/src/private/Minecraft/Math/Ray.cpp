#include "mcpch.h"
#include "Minecraft/Math/Ray.h"


namespace Minecraft {

	bool Ray::WorldIntersection(const Ref<World>& world, RayWorldHitResult& outResult)
	{
		WorldBlockPosition currentBlockPosition = {
			(int)glm::floor(Origin.x),
			(int)glm::floor(Origin.y),
			(uint8_t)glm::floor(Origin.z)
		};

		glm::vec3 absDirection = glm::abs(Direction);

		glm::vec3 rayUnitStepSize;
		if (absDirection.x > absDirection.y && absDirection.x > absDirection.z)
		{
			rayUnitStepSize.x = 1.0f / (Direction.x / Direction.x);
			rayUnitStepSize.y = 1.0f / (Direction.y / Direction.x);
			rayUnitStepSize.z = 1.0f / (Direction.z / Direction.x);
		}
		else if (absDirection.y > absDirection.x && absDirection.y > absDirection.z)
		{
			rayUnitStepSize.x = 1.0f / (Direction.x / Direction.y);
			rayUnitStepSize.y = 1.0f / (Direction.y / Direction.y);
			rayUnitStepSize.z = 1.0f / (Direction.z / Direction.y);
		}
		else
		{
			rayUnitStepSize.x = 1.0f / (Direction.x / Direction.z);
			rayUnitStepSize.y = 1.0f / (Direction.y / Direction.z);
			rayUnitStepSize.z = 1.0f / (Direction.z / Direction.z);
		}

		if (Direction.x < 0)
			rayUnitStepSize.x = -rayUnitStepSize.x;
		if (Direction.y < 0)
			rayUnitStepSize.y = -rayUnitStepSize.y;
		if (Direction.z < 0)
			rayUnitStepSize.z = -rayUnitStepSize.z;

		Int3 step;
		glm::vec3 rayLength1D;

		// Establish starting conditions
		if (Direction.x < 0)
		{
			step.x = -1;
			rayLength1D.x = (Origin.x - (float)currentBlockPosition.x) * rayUnitStepSize.x;
		}
		else
		{
			step.x = 1;
			rayLength1D.x = ((float)(currentBlockPosition.x + 1) - Origin.x) * rayUnitStepSize.x;
		}

		if (Direction.y < 0)
		{
			step.y = -1;
			rayLength1D.y = (Origin.y - (float)currentBlockPosition.y) * rayUnitStepSize.y;
		}
		else
		{
			step.y = 1;
			rayLength1D.y = ((float)(currentBlockPosition.y + 1) - Origin.y) * rayUnitStepSize.y;
		}

		if (Direction.z < 0)
		{
			step.z = -1;
			rayLength1D.z = (Origin.z - (float)currentBlockPosition.z) * rayUnitStepSize.z;
		}
		else
		{
			step.z = 1;
			rayLength1D.z = ((float)(currentBlockPosition.z + 1) - Origin.z) * rayUnitStepSize.z;
		}

		float distanceTraveled = 0.0f;
		while (distanceTraveled < Distance)
		{
			// Check if there is a block at the current position
			if (world->HasBlock(currentBlockPosition))
			{
				outResult.Hit = true;
				outResult.HitBlock = currentBlockPosition;
				outResult.HitFraction = distanceTraveled / Distance;
				return true;
			}

			glm::vec3 absRayLength1D = glm::abs(rayLength1D);

			// Increment block
			if (absRayLength1D.x != 0.0f && (absRayLength1D.x <= absRayLength1D.y || absRayLength1D.y == 0) && (absRayLength1D.x <= absRayLength1D.z || absRayLength1D.z == 0))
			{
				currentBlockPosition.x += step.x;
				distanceTraveled = absRayLength1D.x;
				rayLength1D.x += rayUnitStepSize.x;
			}
			else if (absRayLength1D.y != 0 && (absRayLength1D.y <= absRayLength1D.x || absRayLength1D.x == 0) && (absRayLength1D.y <= absRayLength1D.z || absRayLength1D.z == 0))
			{
				currentBlockPosition.y += step.y;
				distanceTraveled = absRayLength1D.y;
				rayLength1D.y += rayUnitStepSize.y;
			}
			else
			{
				currentBlockPosition.z += step.z;
				distanceTraveled = absRayLength1D.z;
				rayLength1D.z += rayUnitStepSize.z;
			}
		}

		return true;
	}

}
