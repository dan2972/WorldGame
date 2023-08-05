#include "chunk.h"
#include "perlin_generator.h"
#include <random>

Chunk::Chunk(int chunkX, int chunkZ) : m_chunkX{ chunkX }, m_chunkZ{ chunkZ } {
	m_chunk.fill(Air);
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 10);
	
	// for (unsigned i = 0; i < m_chunk.size(); ++i) {
	// 	if (dist6(rng)) {
	// 		m_chunk[i] = Grass;
	// 	}
	// }
	for (int i = 0; i < CHUNK_SIZE; ++i) {
		for (int j = 0; j < CHUNK_SIZE; ++j) {
			float p = 1.0f - PerlinGenerator::getValueAt(chunkX * CHUNK_SIZE + j, chunkZ * CHUNK_SIZE + i, 0.01, 6);
			for (int y = 0; y < (CHUNK_SIZE_Y - 32) + 32 * p; ++y) {
				if (dist6(rng)<2) {
					placeBlock(j, y, i, Stone);
				} else {
					placeBlock(j, y, i, Grass);
				}
			}
			// for (int k = 0; k < CHUNK_SIZE_Y; ++k) {
			// 	if (i < 6 && i >=3 && j < 6 && j >=3 && k < 6 && k >=3)
			// 		placeBlock(j, k, i, Stone);
			// 	if (j == 4 && k == 4 && (i == 2 || i == 6))
			// 		placeBlock(j, k, i, Stone);
			// 	if (j == 4 && i == 4 && (k == 2 || k == 6))
			// 		placeBlock(j, k, i, Stone);
			// 	if (k == 4 && i == 4 && (j == 2 || j == 6))
			// 		placeBlock(j, k, i, Stone);
			// }
		}
	}
}

BlockType Chunk::getBlockAt(unsigned x, unsigned y, unsigned z) const {
	if (y >= CHUNK_SIZE_Y || y < 0)
		return Air;
	return m_chunk[y * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + x];
}

const std::array<BlockType, CHUNK_SIZE* CHUNK_SIZE* CHUNK_SIZE_Y>& Chunk::getBlocks() const {
	return m_chunk;
}

void Chunk::removeBlock(unsigned x, unsigned y, unsigned z) {
	m_chunk[y * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + x] = Air;
}

void Chunk::placeBlock(unsigned x, unsigned y, unsigned z, BlockType type) {
	if(m_chunk[y * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + x] == Air)
		m_chunk[y * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + x] = type;
}

int Chunk::getChunkX() const {
	return m_chunkX;
}

int Chunk::getChunkZ() const {
	return m_chunkZ;
}

int Chunk::getBlockLight(unsigned x, unsigned y, unsigned z) const
{
	return m_lightMap[y * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + x] & 0xF;
}

int Chunk::getSunLight(unsigned x, unsigned y, unsigned z) const
{
	return (m_lightMap[y * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + x] >> 4) & 0xF;
}

void Chunk::setBlockLight(unsigned x, unsigned y, unsigned z, int val)
{
	m_lightMap[y * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + x] = (m_lightMap[y * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + x] & 0xF0) | val;
}

void Chunk::setSunLight(unsigned x, unsigned y, unsigned z, int val)
{
	m_lightMap[y * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + x] = (m_lightMap[y * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + x] & 0xF) | (val << 4);
}
