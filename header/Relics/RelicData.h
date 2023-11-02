#pragma once
#include <cinttypes>

struct RelicData
{
	// The name of the relic
	char m_relicName[32] = "_NAME";
	// The filepath to the relics' icon
	char m_filePath[96] = "_FILEPATH";
	// The description of the relic
	char m_description[359] = "_DESCRIPTION";
	// The price of the relic
	uint8_t m_price = 255;
	// The type(s) of the relic
	uint64_t m_typeFlag = 0x0;
	// The hash of the relic
	uint64_t m_hash = 0xffffffffffffffff;

	// The initialization function of the relic
	void (*m_function)(void*) = nullptr;

	RelicData() = default;
	RelicData(const char name[32], const char path[96], const char desc[359], const uint8_t price, const uint64_t types, void (*init)(void*));
	//RelicData(const RelicData& other);
};

extern RelicData DefaultRelicRock;