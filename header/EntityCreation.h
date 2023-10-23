#pragma once

//Create the entity with its components here
struct EntityID;

//Create round static hazards
EntityID CreateRoundStaticHazard(const char* file, const float& posX, const float& posY, const float& posZ,
	const float& scaleX = 1.0f, const float& scaleY = 1.0f, const float& scaleZ = 1.0f,
	const float& colorRed = .0f, const float& colorGreen = 0.f, const float& colorBlue = 0.f,
	const float& radius = 1.0f, const float& damage = 1.0f, const float& rotationY = -3.1415f * 0.5f);

//Create square static hazards
EntityID CreateSquareStaticHazard(const char* file, const float& posX, const float& posY, const float& posZ,
	const float& scaleX = 1.0f, const float& scaleY = 1.0f, const float& scaleZ = 1.0f,
	const float& corner1X = -1.0f, const float& corner1Y = -1.0f,
	const float& corner2X = 1.0f, const float& corner2Y = -1.0f,
	const float& corner3X = 1.0f, const float& corner3Y = 1.0f,
	const float& corner4X = -1.0f, const float& corner4Y = 1.0f,
	const float& colorRed = .0f, const float& colorGreen = 0.f, const float& colorBlue = 0.f,
	const float& damage = 1.0f, const float& rotationY = -3.1415f * 0.5f);
