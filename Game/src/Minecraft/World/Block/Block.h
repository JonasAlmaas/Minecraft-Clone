#pragma once


namespace Minecraft {

	struct VertexPosition
	{
		// X and Y are 4 bit each
		// Z is 8 bit
		uint8_t BlockX, BlockY, BlockZ;

		bool OffsetX, OffsetY, OffsetZ;

		VertexPosition(uint8_t blockX, uint8_t blockY, uint8_t blockZ, bool offsetX, bool offsetY, bool offsetZ)
			: BlockX(blockX), BlockY(blockY), BlockZ(blockZ), OffsetX(offsetX), OffsetY(offsetY), OffsetZ(offsetZ) {}

		operator uint32_t () { return BlockX + (BlockY << 4) + (BlockZ << 8) + (OffsetX << 16) + (OffsetY << 17) + (OffsetZ << 18); }
	};

	struct VertexColor
	{
		uint8_t R, G, B, I;

		VertexColor(uint8_t r, uint8_t g, uint8_t b, uint8_t i)
			: R(r), G(g), B(b), I(i) {}

		operator uint32_t () { return R + (G << 8) + (B << 16) + (I << 24); }
	};

	class Block
	{
	public:
		enum class Type
		{
			None = 0,
			Air,
			Dirt,
			Grass,
			Stone,
		};

		struct Vertex
		{
			uint32_t LocalPosition;
			uint32_t RGBI;
		};

		struct Position
		{
			uint8_t X, Y, Z;

			Position(uint8_t x, uint8_t y, uint8_t z)
				: X(x), Y(y), Z(z) {}

			bool operator == (const Position& other) const
			{
				return X == other.X && Y == other.Y && Z == other.Z;
			}
		};

	public:
		Block() = default;
		Block(Type type, Int3 position);

		const Int3 GetPosition() const { return m_Position; }

	private:
		Type m_Type = Type::None;
		Int3 m_Position = { 0, 0, 0 };

	};

}

namespace std {

	// X and Y can just be in the range of 0 - 15 (4 bit uint)
	// Z is in the range 0 - 255 (8 bit uint)
	// This means that I can hash the block position buy putting them all in a uint16
	template<>
	struct hash<Minecraft::Block::Position>
	{
		size_t operator()(const Minecraft::Block::Position& key) const
		{
			return hash<uint16_t>()(key.X + (key.Y << 4) + (key.Z << 8));
		}
	};

}
