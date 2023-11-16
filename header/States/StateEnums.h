#pragma once

//Which states (Which registry/registries to have systems go over)
enum State
{
	InMainMenu = 0b1,
	InPlay = 0b10,
	InPause = 0b100,
	InSettings = 0b1000,
	InShop = 0b10000,
	InCredits = 0b100000
};

void SetInMainMenu(bool value);
void SetInPlay(bool value);
void SetInPause(bool value);
void SetInSettings(bool value);
void SetInShop(bool value);
void SetInCredits(bool value);
