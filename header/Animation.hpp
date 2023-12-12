#pragma once
#include "D3D11Helper\IDX_Types.h"
#include "MemLib\PoolPointer.hpp"
#include <DirectXMath.h>
#include "AnimationType.h"

struct AnimationFrame
{
	DirectX::XMMATRIX* vertex = nullptr;
	DirectX::XMMATRIX* normal = nullptr;
};

struct Animation
{
private:
	struct InternalAnimationStructure
	{
		// Used to check if the animation is correctly loaded (little vs big endian) 
		uint32_t m_sanityCheck = 1'234'567'890;
		// The number of bones in a frame
		uint32_t m_boneCount = 0;
		// The number of frames in the animation
		uint32_t m_frameCount = 0;

		// The rest of the data
		// Ignore warning, this is intentional
		#pragma warning(suppress : 4200)
		const char m_data[];
		// vector<float> timestamps
		// vector<Matrix> frames
	};

	PoolPointer<InternalAnimationStructure> m_data;


public:
	bool Load(const char* file);

	// Get timestamps of the frames in the animation
	float GetTimestamp(const uint32_t frameIdx);

	// Get a frame from a time
	const AnimationFrame GetFrame(const float& time);

	~Animation();
};