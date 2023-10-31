#pragma once
#include <cinttypes>

struct RelicData
{
	// The name of the relic
	char m_relicName[32] = "_NAME";
	// The filepath to the relics' icon
	char m_filePath[96] = "_FILEPATH";
	// The description of the relic
	char m_description[375] = "_DESCRIPTION";
	// The price of the relic
	uint8_t m_price = 255;

	// The initialization function of the relic
	void (*m_function)(void*) = nullptr;
};

extern RelicData DefaultRelicRock;