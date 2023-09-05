#pragma once
#define SETTINGS_LIMIT 2//Amount of different kinds of settings
#include<string>
#define SETTINGS_SEPARATOR "="
struct ConfigRow
{
	std::string name;
	int value;
};

struct ConfigRows
{
	ConfigRow rows[SETTINGS_LIMIT];
};

void SetupRows(ConfigRows& rows);