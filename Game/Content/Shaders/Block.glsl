// Main Block shader

#type vertex
#version 450 core

// Block X and Y is in the range of 0 - 15
// Block.Z is in the range of 0 - 255

// Location 0 - Local XYZ
// 4 bit     8 bit    16 bit     24 bit  32 bit
// 4 bit     4 bit     8 bit     8 bit   8 bit
// Block X   Block Y   Block Z   U       V

// Location 1 - RGBI
// 8 bit   16 bit   24 bit   32 bit
// 8 bit   8 bit    8 bit    8 bit
// R       G        B        I


// TODO
// 1 bit          1 bit      1 bit          1 bit      1 bit          1 bit
// SignNormalX    NormalX    SignNormalY    NormalY    SignNormalZ    NormalZ


layout(location = 0) in uint a_LocalXYZ;
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
	// If you shift 1 by 32 you get 0, not what I am after...
	// This took a while to figure out
	// c++ doesn't do this
	// I'll just leave this nice example here...
	//float r = (a_RGBI & ((1 << 8) - 1)) / 255.0;

	// Bit masks
	// 4 = 15
	// 8 = 255
	// 12 = 4095
	// 16 = 65535
	// 20 = 1048575
	// 24 = 16777215
	// 28 = 268435455
	// 32 = 4294967295

	float r = (a_RGBI & 255) / 255.0;					// Bit 0 to 8
	float g = ((a_RGBI & 65535) >> 8) / 255.0;			// Bit 8 to 16
	float b = ((a_RGBI & 16777215) >> 16) / 255.0;		// Bit 16 to 24
	float i = ((a_RGBI & 4294967295) >> 24) / 255.0;	// Bit 24 to 32

	Output.Color = vec4(r, g, b, i);


	// Unpack local position
	uvec3 localPos;
	localPos.x = a_LocalXYZ & 15;
	localPos.y = (a_LocalXYZ & 255) >> 4;
	localPos.z = (a_LocalXYZ & 65535) >> 8;

	// Calculate world position
	vec3 worldPos;
	worldPos.x = float(u_ChunkX * 15) + float(localPos.x);
	worldPos.y = float(u_ChunkY * 15) + float(localPos.y);
	worldPos.z = float(localPos.z);

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
