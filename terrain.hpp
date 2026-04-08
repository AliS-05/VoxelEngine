#pragma once

struct Chunk{
	int worldX;
	int worldZ;
	uint8_t blocks[16][16][16];
};
