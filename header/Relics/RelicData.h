#pragma once
#include <cinttypes>

#define RELIC_DATA_NAME_SIZE (32)
#define RELIC_DATA_PATH_SIZE (96)
#define RELIC_DATA_DESC_SIZE (358)


struct RelicData
{
	// The name of the relic
	char m_relicName[RELIC_DATA_NAME_SIZE] = "_NAME";
	// The filepath to the relics' icon
	char m_filePath[RELIC_DATA_PATH_SIZE] = "_FILEPATH";
	// The description of the relic
	char m_description[RELIC_DATA_DESC_SIZE] = "_DESCRIPTION";
	// The price of the relic
	uint8_t m_price = 255;
	// The weight of the relic
	uint8_t m_weight = 255;
	// The type(s) of the relic
	uint64_t m_typeFlag = 0x0;
	// The hash of the relic
	uint64_t m_hash = 0xffffffffffffffff;

	// The initialization function of the relic
	void (*m_function)(void*) = nullptr;

	RelicData() = default;
	RelicData(const char name[RELIC_DATA_NAME_SIZE], const char path[RELIC_DATA_PATH_SIZE], const char desc[RELIC_DATA_DESC_SIZE], const uint8_t price, const uint64_t types, void (*init)(void*), const uint8_t weight = 255);
	//RelicData(const RelicData& other);
};

extern RelicData DefaultRelicRock;