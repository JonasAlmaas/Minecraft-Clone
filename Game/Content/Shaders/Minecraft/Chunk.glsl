// -- Chunk shader --

#type vertex
#version 450 core

// Block X and Y is in the range of 0 - 15
// Block.Z is in the range of 0 - 255

// Location 0 - Packed local block position and offset
// 4 bit          8 bit          16 bit         17 bit         18 bit         19 bit         20 bit   21 bit
// 4 bit          4 bit          8 bit          1 bit          1 bit          1 bit          1 bit    1 bit
// LocalBlock X   LocalBlock Y   LocalBlock Z   BlockOffsetX   BlockOffsetY   BlockOffsetZ   U        V

// Location 1 - TextureIndex
// 4 bit   8 bit
// 4 bit   4 bit
// X       Y

// Location 2 - RGBI
// 8 bit   16 bit   24 bit   32 bit
// 8 bit   8 bit    8 bit    8 bit
// R       G        B        I


// TODO:
// 1 bit          1 bit      1 bit          1 bit      1 bit          1 bit
// SignNormalX    NormalX    SignNormalY    NormalY    SignNormalZ    NormalZ


layout(location = 0) in uint a_Position;
layout(location = 1) in uint a_TextureIndex;
layout(location = 2) in uint a_RGBI;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

layout(std140, binding = 1) uniform ChunkPosition
{
	int u_ChunkX;
	int u_ChunkY;
};

struct VertexOutput
{
	vec2 UV;
	vec4 Color;
};

layout (location = 0) out VertexOutput Output;

void main()
{
	// -- Unpack UV --
	uvec2 fullUV = uvec2((a_Position & 0x00080000) >> 19, (a_Position & 0x00100000) >> 20);
	uvec2 textureIndex = uvec2(a_TextureIndex & 0x0000000F, (a_TextureIndex & 0x000000F0) >> 4);
	float tileSize = 1.0 / 16.0;

	vec2 uv;
	uv.x = float(textureIndex.x) * tileSize + tileSize * float(fullUV.x);
	uv.y = tileSize * 15.0 + tileSize * float(fullUV.y) - tileSize * float(textureIndex.y);

	/*
	float centerPixleOffset = 1.0 / 512.0;
	vec2 halfPixelCorrection;
	halfPixelCorrection.x = -centerPixleOffset * float(fullUV.x) + centerPixleOffset * float(1 - fullUV.x);
	halfPixelCorrection.y = -centerPixleOffset * float(fullUV.y) + centerPixleOffset * float(1 - fullUV.y);
	uv += halfPixelCorrection;
	*/

	Output.UV = uv;


	// -- Unpack Color --
	float r = (a_RGBI & 0x000000FF) / 255.0;
	float g = ((a_RGBI & 0x0000FF00) >> 8) / 255.0;
	float b = ((a_RGBI & 0x00FF0000) >> 16) / 255.0;
	float i = ((a_RGBI & 0xFF000000) >> 24) / 255.0;

	Output.Color = vec4(r, g, b, i);


	// -- Unpack position --
	uvec3 localBlockPos;
	localBlockPos.x = a_Position & 0x0000000F;
	localBlockPos.y = (a_Position & 0x000000F0) >> 4;
	localBlockPos.z = (a_Position & 0x0000FF00) >> 8;

	uvec3 localBlockOffset;
	localBlockOffset.x = (a_Position & 0x00010000) >> 16;
	localBlockOffset.y = (a_Position & 0x00020000) >> 17;
	localBlockOffset.z = (a_Position & 0x00040000) >> 18;

	// -- Calculate world position --
	vec3 worldPos;
	worldPos.x = float(u_ChunkX * 16 + localBlockPos.x + localBlockOffset.x);
	worldPos.y = float(u_ChunkY * 16 + localBlockPos.y + localBlockOffset.y);
	worldPos.z = float(localBlockPos.z + localBlockOffset.z);

	gl_Position = u_ViewProjection * vec4(worldPos, 1.0);
}


#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

struct VertexOutput
{
	vec2 UV;
	vec4 Color;
};

layout (location = 0) in VertexOutput Input;

layout (binding = 0) uniform sampler2D u_Textures[32];

void main()
{
	o_Color = texture(u_Textures[0], Input.UV);
}
