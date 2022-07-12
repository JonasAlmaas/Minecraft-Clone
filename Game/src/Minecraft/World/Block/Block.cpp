#include "mcpch.h"
#include "Minecraft/World/Block/Block.h"


namespace Minecraft {

	Block::Block(Type type, Int3 position)
		: m_Type(type), m_Position(position)
	{
	}

}
