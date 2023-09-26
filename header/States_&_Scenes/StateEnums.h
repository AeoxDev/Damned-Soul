#pragma once

//Which states (Which registry/registries to have systems go over)
enum State
{
	InMainMenu = 0b1,//Is a registry
	InPlay = 0b10,//Will look at registries
	InPause = 0b100,//Is a registry
	InSettings = 0b1000,//Is a registry
	InShop = 0b10000//In a shop, is registry
};

void SetInMainMenu(bool value);
void SetInPlay(bool value);
void SetInPause(bool value);
void SetInSettings(bool value);
void SetInShop(bool value);
