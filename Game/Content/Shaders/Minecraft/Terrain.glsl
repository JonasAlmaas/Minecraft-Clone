// -- Terrain shader --

#type vertex
#version 450 core

// Block X and Y is in the range of 0 - 15
// Block.Z is in the range of 0 - 255

// Location 0 - Pack 1
// 4 bit          8 bit          16 bit         17 bit         18 bit         19 bit         20 bit   21 bit   25 bit      29 bit      30 bit    31 bit    32 bit
// 4 bit          4 bit          8 bit          1 bit          1 bit          1 bit          1 bit    1 bit    bit 4       bit 4       bit 1     bit 1     bit 1
// LocalBlock X   LocalBlock Y   LocalBlock Z   BlockOffsetX   BlockOffsetY   BlockOffsetZ   U        V		   TexIndexX   TexIndexY   NormalX   NormalZ   NormalZ

// Might be able to get the normal from the block ffset

// Location 2 - RGBV
// 8 bit   16 bit   24 bit   32 bit
// 8 bit   8 bit    8 bit    8 bit
// R       G        B        I

layout(location = 0) in uint a_Pack1;
layout(location = 1) in uint a_RGBV;

layout(std140, binding = 1) uniform Camera
{
	mat4 u_ViewProjection;
};

layout(std140, binding = 2) uniform ChunkPosition
{
	int u_ChunkX;
	int u_ChunkY;
};

struct VertexOutput
{
	vec2 UV;
	vec4 Color;
	vec3 Normal;
};

layout (location = 0) out VertexOutput Output;

void main()
{
	// Unpack position
	uvec3 localBlockPos;
	localBlockPos.x = a_Pack1 & 0xF;
	localBlockPos.y = (a_Pack1 & 0xF0) >> 4;
	localBlockPos.z = (a_Pack1 & 0xFF00) >> 8;

	// Unpack block offset
	uvec3 blockOffset;
	blockOffset.x = (a_Pack1 & 0x10000) >> 16;
	blockOffset.y = (a_Pack1 & 0x20000) >> 17;
	blockOffset.z = (a_Pack1 & 0x40000) >> 18;

	// Unpack UV - U and V can only be either 0 or 1
	uvec2 fullUV = uvec2((a_Pack1 & 0x80000) >> 19, (a_Pack1 & 0x100000) >> 20);

	// Unpack texture index
	uvec2 textureIndex = uvec2((a_Pack1 & 0x1E00000) >> 21, (a_Pack1 & 0x1E000000) >> 25);

	float tileSize = 1.0 / 16.0;

	// Get a uv depending on the full uv and the texture index
	vec2 uv;
	uv.x = float(textureIndex.x) * tileSize + tileSize * float(fullUV.x);
	uv.y = tileSize * 15.0 + tileSize * float(fullUV.y) - tileSize * float(textureIndex.y);
	Output.UV = uv;

	// Unpack Normal
	vec3 normal;
	uint normalIndex = (a_Pack1 & 0xE0000000) >> 29;
	switch (normalIndex)
	{
		case 0: { normal = vec3( 0,  0,  1); break; }		// Top
		case 1:	{ normal = vec3( 0,  0, -1); break; }		// Bottom
		case 2: { normal = vec3( 0,  1,  0); break; }		// North
		case 3: { normal = vec3( 0, -1,  0); break; }		// South
		case 4: { normal = vec3( 1,  0,  0); break; }		// East
		case 5: { normal = vec3(-1,  0,  0); break; }		// West
		default: { normal = vec3(1,  1,  1); break; }		// Something is wrong!
	}
	Output.Normal = normal;

	// Unpack Color
	float r = (a_RGBV & 0xFF) / 255.0;
	float g = ((a_RGBV & 0xFF00) >> 8) / 255.0;
	float b = ((a_RGBV & 0xFF0000) >> 16) / 255.0;
	float v = ((a_RGBV & 0xFF000000) >> 24) / 255.0;
	Output.Color = vec4(r, g, b, v);

	// Calculate world position
	vec3 worldPos;
	worldPos.x = float(u_ChunkX * 16 + int(localBlockPos.x + blockOffset.x));
	worldPos.y = float(u_ChunkY * 16 + int(localBlockPos.y + blockOffset.y));
	worldPos.z = float(localBlockPos.z + blockOffset.z);

	gl_Position = u_ViewProjection * vec4(worldPos, 1.0);
}


#type pixel
#version 450 core

layout(location = 0) out vec4 o_Color;

struct VertexOutput
{
	vec2 UV;
	vec4 Color;
	vec3 Normal;
};

layout (location = 0) in VertexOutput Input;

layout (binding = 0) uniform sampler2D u_Textures[32];

float Lerp(float x, float y, float a)
{
	return x + ((y - x) * a);
}

vec3 Lerp3(vec3 v1, vec3 v2, float a)
{
	return vec3(Lerp(v1.x, v2.x, a), Lerp(v1.y, v2.y, a), Lerp(v1.z, v2.z, a));
}

void main()
{
	vec4 color = texture(u_Textures[0], Input.UV);
	o_Color = vec4(Lerp3(color.xyz, Input.Color.xyz, Input.Color.a), color.a);

	vec3 lightDir = normalize(vec3(-1, -1, -1));
	float d = max(dot(Input.Normal, -lightDir), 0.0) * 0.25 + 0.75;

	o_Color = vec4(o_Color.r * d, o_Color.g * d, o_Color.b * d, o_Color.a);
}
