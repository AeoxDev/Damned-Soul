#pragma once
#define SETTINGS_LIMIT 2//Amount of different kinds of settings
#include<string>
#define SETTINGS_SEPARATOR " "

//NOTE: Come back to when Configurations is more fleshed out
//Add Windowed, Borderless, Fullscreen
//Add Master, SFX, Music
//Add Keybinds
// 
//Add Graphical settings?
//Add Accessibility settings?
//Add Mouse sensitivity (Requires Custom mouse code)?

struct ConfigRow
{
	std::string name;
	std::string value;
};

struct ConfigRows
{
	ConfigRow rows[SETTINGS_LIMIT];
};

void SetupRows(ConfigRows& rows);

/// <summary>///Interpret name and send value in correct format to respective recipient/// </summary>/// <param name="name"></param>/// <param name="value"></param>
void InterpretRow(const std::string& name, const std::string& value);