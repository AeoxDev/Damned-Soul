#include "Animation.hpp"
#include "MemLib\ML_String.hpp"
#include <fstream>

bool Animation::Load(const char* filename)
{
	//ML_String name = "Models\\Ani\\";
	//name.append(filename);

	std::ifstream reader;
	auto flags = std::ios::binary; // | std::ios::ate;
	reader.open(filename/*name.c_str()*/, flags);

	assert(true == reader.is_open());

	// Allocate temporarily onto the stack
	reader.seekg(0, std::ios::end);
	unsigned int size = static_cast<unsigned int>(reader.tellg());
	char* modelData = (char*)MemLib::spush(size);
	reader.seekg(0, std::ios::beg);

	// Read byte data onto stack
	reader.read(modelData, size);
	reader.close();

	// Copy data to the boneless model
	m_data = MemLib::palloc(size);
	std::memcpy(&(*m_data), modelData, size);

	assert(m_data->m_sanityCheck == 1'234'567'890);

	// pop the stack
	MemLib::spop();

	return true;
}

float Animation::GetTimestamp(const uint32_t frameIdx)
{
    if (frameIdx < m_data->m_frameCount)
    {
        return ((float*)m_data->m_data)[frameIdx];
		//return m_data->timestamps[frameIdx];
    }
	return 0;
}

AnimationFrame Animation::GetFrame(const float& time)
{
	AnimationFrame retVal;

	if (m_data->m_boneCount)
	{
		uint32_t i;
		// Find the most appropriate frame
		for (i = 0; (i < m_data->m_frameCount - 1) && GetTimestamp(i) < time; ++i) {}

		// Vertex bone transformations
		retVal.vertex = (((DirectX::XMMATRIX*)&((float*)m_data->m_data)[m_data->m_frameCount]) + i * m_data->m_boneCount);
		// Normal bone transformations
		retVal.normal = (((DirectX::XMMATRIX*)&((float*)m_data->m_data)[m_data->m_frameCount]) + (i * m_data->m_boneCount) + (m_data->m_boneCount * m_data->m_frameCount));
	}

	return retVal;
}

Animation::~Animation()
{
	MemLib::pfree(m_data);
}
