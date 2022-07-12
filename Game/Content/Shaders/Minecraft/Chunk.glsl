// Main chunk shader

#type vertex
#version 450 core

// Block X and Y is in the range of 0 - 15
// Block.Z is in the range of 0 - 255

// Location 0 - Packed local block position and offset
// 4 bit          8 bit          16 bit         17 bit         18 bit         19 bit
// 4 bit          4 bit          8 bit          1 bit          1 bit          1 bit
// LocalBlock X   LocalBlock Y   LocalBlock Z   BlockOffsetX   BlockOffsetY   BlockOffsetZ

// Location 1 - RGBI
// 8 bit   16 bit   24 bit   32 bit
// 8 bit   8 bit    8 bit    8 bit
// R       G        B        I


// TODO:
// 1 bit          1 bit      1 bit          1 bit      1 bit          1 bit
// SignNormalX    NormalX    SignNormalY    NormalY    SignNormalZ    NormalZ


layout(location = 0) in uint a_Position;
layout(location = 1) in uint a_RGBI;

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
	vec4 Color;
};

layout (location = 0) out VertexOutput Output;

void main()
{
	// -- Unpack Color --
	float r = (a_RGBI & 0x000000FF) / 255.0;			// Bit 0 to 8
	float g = ((a_RGBI & 0x0000FF00) >> 8) / 255.0;		// Bit 8 to 16
	float b = ((a_RGBI & 0x00FF0000) >> 16) / 255.0;	// Bit 16 to 24
	float i = ((a_RGBI & 0xFF000000) >> 24) / 255.0;	// Bit 24 to 32

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
	vec4 Color;
};

layout (location = 0) in VertexOutput Input;

void main()
{
	o_Color = Input.Color;
}
