#include "ConfigManager.h"
#include "ConfigInterpreter.h"
#include <fstream>
#include <filesystem>
#include <iostream>

void DefaultConfig()
{
	//Create file
	std::fstream newFile{ "Settings.config", newFile.out };

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
			confLine = configs.rows[i].name + SETTINGS_SEPARATOR;
			confLine += configs.rows[i].value + "\n";
			newFile.write(confLine.c_str(), confLine.size());
		}

		newFile.close();
	}
}


void InitiateConfig()
{
	//First check if config file exists
	//If it does, do nothing
	//If it does'nt, create a config file.
	std::filesystem::path checkPath{ "Settings.config" };
	if (!std::filesystem::exists(checkPath))
	{
		DefaultConfig();
	}
	else
	{
		//Open the file
		std::fstream configFile{ "Settings.config", configFile.in };

		if (!configFile.is_open())
		{
			//Failed
			std::cout << "WARNING! Failed to open Settings.config" << std::endl;
		}
		else
		{
			int i = 0;
			while (!configFile.eof() && i < SETTINGS_LIMIT)
			{
				std::string confLine;
				ConfigRows configs;
				SetupRows(configs);
				std::getline(configFile, confLine);

				//After reading the line, seperate by spaces
				std::stringstream ss(confLine);

				std::string name, value;
				ss >> name >> value;
				if (configs.rows[i++].name != name)
				{
					configFile.close();
					//Something has changed (corruption), we should default.
					DefaultConfig();

					return;
				}
				//Handle each case by name.
				InterpretRow(name, value);
			}

		}
		configFile.close();
	}
	//Else nothing

}

