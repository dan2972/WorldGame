#include "chunk.h"
#include "perlin_generator.h"
#include <random>

Chunk::Chunk(int chunkX, int chunkZ) : m_chunkX{ chunkX }, m_chunkZ{ chunkZ } {
	m_chunk.fill(Air);

	for (int i = 0; i < CHUNK_SIZE; ++i) {
		for (int j = 0; j < CHUNK_SIZE; ++j) {
			float p = 1.0f - PerlinGenerator::getValueAt(chunkX * CHUNK_SIZE + j, chunkZ * CHUNK_SIZE + i, 0.01, 6);
			int height = (CHUNK_SIZE_Y - 32) + 32 * p;

			for (int y = 0; y < height; ++y) {
				float x = (chunkX * CHUNK_SIZE + j) * 7.0f;
				float z = (chunkZ * CHUNK_SIZE + i) * 7.0f;
				float p2 = PerlinGenerator::getValueAt(x, y * 10.0f, z, 0.001, 5) * 2 - 1;
				float val = (std::abs(p2));
				float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				if (r < 1.0f-std::max(0.0f, (y-1) / 10.0f)) {
					placeBlock(j, y, i, Bedrock);
				}else if (y < height - 8 - r * 2) {
					placeBlock(j, y, i, Stone);
				} if (y < height - 3- r * 2) {
					placeBlock(j, y, i, Dirt);
				} else {
					placeBlock(j, y, i, Grass);
				}
				//((float)(height + 50 - y) / (height + 50)) * 0.1f
				if (y > 13 && y < height - 10 && val <= ((float)(height - y) / (height)) * 0.2f) {
					removeBlock(j, y, i);
				}
				// if (r < 1.0f-std::max(0.0f, (y-1) / 10.0f)) {
				// 	placeBlock(j, y, i, Bedrock);
				// } if (r < std::max(0.0f, ((float)((height - y - 2)))/(height * 0.05f))) {
				// 	placeBlock(j, y, i, Stone);
				// } else {
				// 	placeBlock(j, y, i, Grass);
				// }
			}
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
