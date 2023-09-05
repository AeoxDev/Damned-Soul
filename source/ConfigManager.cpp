#include "ConfigManager.h"
#include <fstream>
#include <filesystem>
#include "ConfigRows.h"
#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG




void InitiateConfig()
{
	//First check if config file exists
	//If it does, do nothing
	//If it does'nt, create a config file.
	std::filesystem::path checkPath{ "Settings.config" };
	if (!std::filesystem::exists(checkPath))
	{
		//Create file
		std::fstream newFile{"Settings.config", newFile.in};

		if (!newFile.is_open())
		{
			//Failed
			std::cout << "WARNING! Failed to open Settings.config" << std::endl;
		}
		else
		{
			ConfigRows configs;
			SetupRows(configs);
			std::string confLine;
			for (size_t i = 0; i < SETTINGS_LIMIT; i++)
			{
				confLine = configs.rows[0].name + SETTINGS_SEPARATOR;
				confLine+= configs.rows[0].value;
				newFile.write(confLine.c_str(), confLine.size());
			}
			
		}
	}
	else
	{

	}
	//Else nothing

}
