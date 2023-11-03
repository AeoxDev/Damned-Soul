#include "Relics/RelicData.h"
#include "Hashing.h"
#include <cstring>

RelicData::RelicData(const char name[32], const char path[96], const char desc[359], const uint8_t price, const uint64_t types, void (*init)(void*))
{
	std::memcpy(m_relicName, name, 32);
	std::memcpy(m_filePath, path, 96);
	std::memcpy(m_description, desc, 359);
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
