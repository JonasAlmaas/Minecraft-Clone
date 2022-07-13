#pragma once

#include "Minecraft/World/Block/Block.h"


namespace Minecraft {

	class DirtBlock : public Block
	{
	public:
		DirtBlock() = default;

		virtual uint16_t GetTextureIndex(Face face) const override;
		virtual Renderpass GetRenderpass() const override { return Renderpass::Solid; }

	};

}
