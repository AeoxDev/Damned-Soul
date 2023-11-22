#pragma once

struct ZacBehaviour
{
	int zacIndex = 0;
	float targetX = 0;
	float targetZ = 0;


	ZacBehaviour(int index, float x, float z) : zacIndex(index), targetX(x), targetZ(z) {}
};