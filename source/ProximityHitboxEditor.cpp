#include "ProximityHitboxEditor.h"
#include "Input.h"
#include <sstream>
#include <iostream>

void HitboxEditorLoop(std::string fileName)
{
	CurrentHitboxVariables CHV;
	bool done = false;

	//Read hitbox file if it exists
	fileName = "HitboxFiles/" + fileName + ".box";
	std::ifstream file(fileName);
	if (file.is_open())
	{
		//Read file contents and add to CHV
		std::string line = "";
		std::string value = "";
		ProximityPoint currentPoint;
		while (std::getline(file, line))
		{
			std::stringstream ss(line);

			//Get prefix
			std::string prefix;
			ss >> prefix;

			if (prefix == "Point")
			{
				//Set currentPoint index to value
				ss >> value;
				currentPoint.index = std::stoi(value);
			}
			else if (prefix == "X")
			{
				ss >> value;
				currentPoint.x = std::stoi(value);
			}
			else if (prefix == "Z")
			{
				ss >> value;
				currentPoint.z = std::stoi(value);
				CHV.list.push_back(currentPoint);
			}
			else if (prefix == "Clockwise")
			{
				ss >> value;
				CHV.clockwise = std::stoi(value);
			}
		}
		file.close();
	}

	//If points exist, turn them from world space to screen space.

	int DeleteCounter = 0;
	int CreateCounter = 0;

	while (!done)
	{
		//Render the entire level as a top down view

		//Render the placed out points

		//Get click event and mouse position
		GetInput();

		if (mouseButtonPressed[MouseButton::right] == pressed)
		{
			std::cout << "Right Click pressed" << std::endl;
			bool found = false;
			//Check if the mouse is within the radius of a point
			for (int i = 0; i < (int)CHV.list.size(); i++)
			{
				if ((abs(CHV.list[i].x - (float)mouseX) <= 5.0f) && (abs(CHV.list[i].z - (float)mouseY) <= 5.0f)) //If within radius
				{
					std::cout << "Within radius" << std::endl;
					//Delete selected point
					found = true;
					CHV.list.erase(i);
					for (int i = 0; i < (int)CHV.list.size(); i++)
					{
						CHV.list[i].index = i;
					}
					break;
				}
			}

			//If no point close enough was found, remove the last placed element
			if (!found)
			{
				CHV.list.pop_back();
				std::cout << "Remove last point" << std::endl;
			}

			while (1)
			{
				GetInput();
				if (mouseButtonPressed[MouseButton::right] == released)
				{
					break;
				}
			}
		}

			//If left click && within a point's radius --> Move selected point.
			//Else if left click && not within a point's radius --> Place out a new point.
		if (mouseButtonPressed[MouseButton::left] == pressed)
		{
			std::cout << "Left Click pressed" << std::endl;
			bool found = false;
			//Check if the mouse is within the radius of a point
			for (int i = 0; i < (int)CHV.list.size(); i++)
			{
				if ((abs(CHV.list[i].x - (float)mouseX) <= 5.0f) && (abs(CHV.list[i].z - (float)mouseY) <= 5.0f)) //If within radius
				{
					found = true;
					while (mouseButtonPressed[MouseButton::left] == down)
					{
						//Check until mouse is released
						GetInput();
						if (mouseButtonPressed[MouseButton::left] == released)
						{
							break;
						}
					}
					//Move point to where down was released
					std::cout << "Within radius and released" << std::endl;
					CHV.list[i].x = (float)mouseX;
					CHV.list[i].z = (float)mouseY;
					break;
				}
			}

			//If no point close enough was found, create a new point at current location
			if (!found)
			{
				std::cout << "Add new point" << std::endl;
				//Create point
				ProximityPoint toAdd;
				toAdd.index = CHV.list.size();
				toAdd.x = (float)mouseX;
				toAdd.z = (float)mouseY;

				CHV.list.push_back(toAdd);

				while (1)
				{
					GetInput();
					if (mouseButtonPressed[MouseButton::left] == released)
					{
						break;
					}
				}
			}
		}


		//If Enter is pressed twice handle file creation/editing depending on fileName
		if (keyState[Keys::SCANCODE_RETURN] == pressed)
		{
			std::cout << "pressed Return" << std::endl;
			DeleteCounter = 0;
			CreateCounter++;
			std::cout << CreateCounter << std::endl;
			if (CreateCounter > 1)
			{
				std::ifstream file(fileName);
				if (file.is_open())
				{
					file.close();
					//Delete the file before remaking it
					remove(fileName.c_str());
				}

				//Create file
				std::ofstream outFile(fileName);
				if (outFile.is_open())
				{
					//Write the contents of the file (Remember to apply world matrix to output the points in world space):
					outFile << "Clockwise " << CHV.clockwise << "\n";
					for (int i = 0; i < (int)CHV.list.size(); i++)
					{
						outFile << "Point " << CHV.list[i].index << "\n";
						outFile << "X " << CHV.list[i].x << "\n";
						outFile << "Z " << CHV.list[i].z << "\n";
					}
					std::cout << "Created new file" << std::endl;
					done = true;
				}
			}

			while (1)
			{
				GetInput();
				if (keyState[Keys::SCANCODE_RETURN] == released)
				{
					break;
				}
			}
		}
		
		//If Backspace is pressed twice handle file removal
		if (keyState[Keys::SCANCODE_BACKSPACE] == pressed)
		{
			CreateCounter = 0;
			DeleteCounter++;
			if (DeleteCounter > 1)
			{
				std::ifstream file(fileName);
				if (file.is_open())
				{
					file.close();
					//Delete the file
					remove(fileName.c_str());
					std::cout << "Removed file" << std::endl;
				}
				done = true;
			}

			while (1)
			{
				GetInput();
				if (keyState[Keys::SCANCODE_BACKSPACE] == released)
				{
					break;
				}
			}
		}
	}
}
