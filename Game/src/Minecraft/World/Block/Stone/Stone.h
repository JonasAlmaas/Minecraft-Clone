#pragma once

#include "Minecraft/World/Block/Block.h"


namespace Minecraft {

	class StoneBlock : public Block
	{
	public:
		StoneBlock() = default;

		virtual uint16_t GetTextureIndex(Face face) const override;
		virtual Renderpass GetRenderpass() const override { return Renderpass::Solid; }

	};

}
