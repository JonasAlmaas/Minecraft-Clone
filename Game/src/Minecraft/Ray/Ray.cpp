#include "mcpch.h"
#include "Minecraft/Ray/Ray.h"


namespace Minecraft {

	float Ray::WorldIntersection(const Ref<World>& world, RayWorldHitResult& outResult)
	{
		glm::vec2 rayUnitStepSize = {
			sqrt(1.0f + (Direction.y / Direction.x) * (Direction.y / Direction.x)),
			sqrt(1.0f + (Direction.x / Direction.y) * (Direction.x / Direction.y))
		};

		Int3 currentBlockPosition = { (int)glm::floor(Origin.x), (int)glm::floor(Origin.y), (int)glm::floor(Origin.z) };
		glm::vec2 rayLength1D;
		Int2 step;

		// Establish Starting Conditions
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

		float distanceTraveled = 0.0f;
		while (distanceTraveled < Distance)
		{
			if (rayLength1D.x < rayLength1D.y)
			{
				currentBlockPosition.x += step.x;
				distanceTraveled = rayLength1D.x;
				rayLength1D.x += rayUnitStepSize.x;
			}
			else
			{
				currentBlockPosition.y += step.y;
				distanceTraveled = rayLength1D.y;
				rayLength1D.y += rayUnitStepSize.y;
			}

			// Check if there is a block at the current position
			Chunk::Position chunkPosition = {
				(int)glm::floor((float)currentBlockPosition.x / 16.0f),
				(int)glm::floor((float)currentBlockPosition.y / 16.0f)
			};

			if (world->HasChunk(chunkPosition))
			{
				ChunkBlock::Position localBlockPosition = {
					currentBlockPosition.x % 16,
					currentBlockPosition.y % 16,
					currentBlockPosition.z
				};

				if (world->GetChunk(chunkPosition)->HasBlock(localBlockPosition))
				{
					outResult.Hit = true;
					outResult.HitBlock = currentBlockPosition;
					outResult.HitFraction = distanceTraveled / Distance;
					return outResult.HitFraction;
				}
			}
		}

		return 0.0f;
	}

}
