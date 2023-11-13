#include "Relics/RelicData.h"
#include "Hashing.h"
#include <cstring>

RelicData::RelicData(const char name[RELIC_DATA_NAME_SIZE], const char path[RELIC_DATA_PATH_SIZE], const char desc[RELIC_DATA_DESC_SIZE], const uint8_t price, const uint64_t types, void (*init)(void*))
{
	std::memcpy(m_relicName, name, RELIC_DATA_NAME_SIZE);
	std::memcpy(m_filePath, path, RELIC_DATA_PATH_SIZE);
	std::memcpy(m_description, desc, RELIC_DATA_DESC_SIZE);
	m_price = price;
	m_typeFlag = types;
	m_function = init;
	m_hash = C_StringToHash(m_relicName);
}

//RelicData::RelicData(const RelicData& other)
//{
//	std::memcpy(m_relicName, other.m_filePath, 32);
//	std::memcpy(m_filePath, other.m_filePath, 96);
//	std::memcpy(m_description, other.m_description, 359);
//	m_price = other.m_price;
//	m_typeFlag = other.m_typeFlag;
//	m_function = other.m_function;
//	m_hash = other.m_hash;
//}
