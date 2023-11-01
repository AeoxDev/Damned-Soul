#pragma once
#include "UI.h"

struct OnHoverComponent
{
	DSFLOAT2 position = { 0.0f, 0.0f };
	DSBOUNDS bounds = { 0.0f, 0.0f, 0.0f, 0.0f };

	bool redrawUI = true;
	bool hasBeenDrawn = false;

	void(*onHover)(void*, bool);

	void Setup(DSFLOAT2 pos, DSBOUNDS bnds, void(*func)(void*, bool))
	{
		position = pos;
		bounds = bnds;
		onHover = func;
	};

	bool Intersect(DSINT2 mousePosition)
	{
		return (mousePosition.x > position.x) && (mousePosition.x < position.x + bounds.right) &&
			(mousePosition.y > position.y) && (mousePosition.y < position.y + bounds.bottom);
	};
};