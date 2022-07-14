#pragma once


namespace Minecraft {

	struct Int2
	{
		int X, Y;

		Int2() : X(0), Y(0) {}
		Int2(int x, int y) : X(x), Y(y) {}

		bool operator == (const Int2& other) const { return X == other.X && Y == other.Y; }
		bool operator != (const Int2& other) const { return !(*this == other); }

		Int2 operator + (const Int2& other) const { return { X + other.X, Y + other.Y }; }
		Int2 operator - (const Int2& other) const { return { X - other.X, Y - other.Y }; }

		Int2 operator + (int value) const { return { X + value, Y + value }; }
		Int2 operator - (int value) const { return { X - value, Y - value }; }
	};

	struct Int3
	{
		int X, Y, Z;

		Int3() : X(0), Y(0), Z(0) {}
		Int3(int x, int y, int z) : X(x), Y(y), Z(z) {}

		bool operator == (const Int3& other) const { return X == other.X && Y == other.Y && Z == other.Z; }
		bool operator != (const Int3& other) const { return !(*this == other); }

		Int3 operator + (const Int3& other) const { return { X + other.X, Y + other.Y, Z + other.Z }; }
		Int3 operator - (const Int3& other) const { return { X - other.X, Y - other.Y, Z - other.Z }; }

		Int3 operator + (int value) const { return { X + value, Y + value, Z + value }; }
		Int3 operator - (int value) const { return { X - value, Y - value, Z - value }; }
	};

}
