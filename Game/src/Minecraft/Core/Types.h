#pragma once

#include <xhash>


namespace Minecraft {

	struct Int2
	{
		int X, Y;

		Int2(int x, int y)
			: X(x), Y(y) {}

		bool operator == (const Int2& other) const
		{
			return X == other.X && Y == other.Y;
		}
	};

	struct Int3
	{
		int X, Y, Z;

		Int3(int x, int y, int z)
			: X(x), Y(y), Z(z) {}

		bool operator == (const Int3& other) const
		{
			return X == other.X && Y == other.Y && Z == other.Z;
		}
	};

}

namespace std {

	// TODO: This doesn't really work at all :D

	template<>
	struct hash<Minecraft::Int2>
	{
		size_t operator()(const Minecraft::Int2& key) const
		{
			return hash<int>()(key.X);
		}
	};

	template<>
	struct hash<Minecraft::Int3>
	{
		size_t operator()(const Minecraft::Int3& key) const
		{
			return hash<int>()(key.X);
		}
	};

}
