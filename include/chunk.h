#ifndef CHUNK_H
#define CHUNK_H

#include <array>
#include "block_type.h"

static const int CHUNK_SIZE = 16;
static const int CHUNK_SIZE_Y = 256;

class Chunk
{
public:
	Chunk(int chunkX, int chunkZ);

	BlockType getBlockAt(unsigned x, unsigned y, unsigned z) const;
	void placeBlock(unsigned x, unsigned y, unsigned z, BlockType type);
	void removeBlock(unsigned x, unsigned y, unsigned z);

	const std::array<BlockType, CHUNK_SIZE* CHUNK_SIZE* CHUNK_SIZE_Y>& getBlocks() const;

	int getChunkX() const;
	int getChunkZ() const;

	int getBlockLight(unsigned x, unsigned y, unsigned z) const;
	int getSunLight(unsigned x, unsigned y, unsigned z) const;
	void setBlockLight(unsigned x, unsigned y, unsigned z, int val);
	void setSunLight(unsigned x, unsigned y, unsigned z, int val);

	bool requestedUpdate = false;

private:
	std::array<BlockType, CHUNK_SIZE* CHUNK_SIZE* CHUNK_SIZE_Y> m_chunk{};
	std::array<char, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE_Y> m_lightMap{};
	int m_chunkX, m_chunkZ;
};

#endif