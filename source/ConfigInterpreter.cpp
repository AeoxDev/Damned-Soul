#include "ConfigInterpreter.h"
#include "SDLHandler.h"

//NOTE: Come back to when Configurations is more fleshed out


void InterpretRow(const std::string& name, const std::string& value)
{
	if (name == "ResolutionX")
	{
		//Convert Value to int
		sdl.WIDTH = (unsigned)std::stoi(value);
		sdl.BASE_WIDTH = (unsigned)std::stoi(value);
	}
	else if(name == "ResolutionY")
	{
		//Convert Value to int
		sdl.HEIGHT = (unsigned)std::stoi(value);
		sdl.BASE_HEIGHT = (unsigned)std::stoi(value);
	}
	else if (name == "WindowMode")
	{
		if (value == "Windowed")
		{
			sdl.windowFlags = 0;
		}
		else if (value == "Borderless")
		{
			sdl.windowFlags = SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		else if (value == "Fullscreen")
		{
			sdl.windowFlags = SDL_WINDOW_FULLSCREEN;
		}
	}
}

void SetupRows(ConfigRows& rows)
{
	//Create ConfigRow:s and add to ConfigRows
	rows.rows[0] = ConfigRow{ "ResolutionX", "1920" };//Name, default value
	rows.rows[1] = ConfigRow{ "ResolutionY", "1030" };
	rows.rows[2] = ConfigRow{ "WindowMode", "Windowed" };
}
