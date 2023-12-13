#include "ProximityHitboxEditor.h"
#include "Input.h"
#include <sstream>
#include <iostream>

#include <string>

void HitboxEditorLoop(const char* fileName, bool clockwise)
{
	CurrentHitboxVariables CHV;
	bool done = false;

	std::string tempFilename = fileName; //Herman Help;
	std::string tempFilename2 = "HitboxFiles/" + tempFilename + ".box"; //Herman Help;

	//Read hitbox file if it exists
	fileName = tempFilename2.c_str();
	std::ifstream file(fileName);
	if (file.is_open())
	{
		//Read file contents and add to CHV
		std::string line = "";
		std::string value = "";
		ProximityPointEdit currentPoint;
		while (std::getline(file, line))
		{
			std::stringstream ss(line);

			//Get prefix
			std::string prefix;
			ss >> prefix;

			if (prefix == "X")
			{
				ss >> value;
				currentPoint.x = (float)std::stoi(value);
			}
			else if (prefix == "Z")
			{
				ss >> value;
				currentPoint.z = (float)std::stoi(value);
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

	//Set clockwise/counter-clockwise
	if (clockwise)
	{
		CHV.clockwise = 1;
	}
	else
	{
		CHV.clockwise = 0;
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
			bool found = false;
			//Check if the mouse is within the radius of a point
			for (int i = 0; i < (int)CHV.list.size(); i++)
			{
				if ((abs(CHV.list[i].x - (float)mouseX) <= 5.0f) && (abs(CHV.list[i].z - (float)mouseY) <= 5.0f)) //If within radius
				{
					std::cout << "Removed selected point" << std::endl;
					//Delete selected point
					found = true;
					CHV.list.erase(i);
					break;
				}
			}

			//If no point close enough was found, remove the last placed element
			if (!found && CHV.list.size() > 0)
			{
				CHV.list.pop_back();
				std::cout << "Removed last point" << std::endl;
			}

			while (1)
			{
				GetInput();
				if (mouseButtonPressed[MouseButton::right] == released)
				{
					break;
				}
			}
			//Check for edge cases
			if (CHV.list.size() > 2)
			{
				EdgeCaseFixer(CHV, false);
			}
		}

			//If left click && within a point's radius --> Move selected point.
			//Else if left click && not within a point's radius --> Place out a new point.
		if (mouseButtonPressed[MouseButton::left] == pressed)
		{
			bool found = false;
			//Check if the mouse is within the radius of a point
			for (int i = 0; i < (int)CHV.list.size(); i++)
			{
				if ((abs(CHV.list[i].x - (float)mouseX) <= 5.0f) && (abs(CHV.list[i].z - (float)mouseY) <= 5.0f)) //If within radius
				{
					std::cout << "Move selected point" << std::endl;
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
					std::cout << "Moved point to mouse" << std::endl;
					CHV.list[i].x = (float)mouseX;
					CHV.list[i].z = (float)mouseY;
					break;
				}
			}

			//If no point close enough was found, create a new point at current location
			if (!found)
			{
				std::cout << "Added new point" << std::endl;
				//Create point
				ProximityPointEdit toAdd;
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
			//Check for edge cases
			if (CHV.list.size() > 2)
			{
				EdgeCaseFixer(CHV, false);
			}
		}


		//If Enter is pressed twice handle file creation/editing depending on fileName
		if (keyState[Keys::SCANCODE_RETURN] == pressed)
		{
			DeleteCounter = 0;
			CreateCounter++;

			if (CHV.list.size() > 100)
			{
				std::cout << "There are currently more than 100 points, are you sure you want to have this many?" << std::endl;
			}

			if (CreateCounter > 1)
			{
				//Check for edge cases
				if (CHV.list.size() > 2)
				{
					EdgeCaseFixer(CHV, true);
				}

				std::ifstream file(fileName);
				if (file.is_open())
				{
					file.close();
					//Delete the file before remaking it
					remove(fileName);
				}

				//Create file
				std::ofstream outFile(fileName);
				if (outFile.is_open())
				{
					//Write the contents of the file (Remember to apply world matrix to output the points in world space):
					outFile << "Clockwise " << CHV.clockwise << "\n";
					for (int i = 0; i < (int)CHV.list.size(); i++)
					{
						outFile << "Point " << i << "\n";
						outFile << "X " << CHV.list[i].x << "\n";
						outFile << "Z " << CHV.list[i].z << "\n";
					}
					std::cout << "Created new file" << std::endl;
					done = true;
				}
			}
			else
			{
				std::cout << "Press Return again to confirm creation" << std::endl;
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
					remove(fileName);
					std::cout << "Removed file" << std::endl;
				}
				done = true;
			}
			else
			{
				std::cout << "Press Backspace again to confirm deletion" << std::endl;
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

void EdgeCaseFixer(CurrentHitboxVariables& CHV, bool lastIncluded)
{
	int currentSize = 0;
	if (lastIncluded)
	{
		currentSize = (int)CHV.list.size();
	}
	else
	{
		currentSize = (int)CHV.list.size() - 1;
	}

	//Loop through the list and check if the middle point of lines are closer to another point not connected to that line
	for (int i = 0; i < currentSize; i++)
	{
		//Create point that's in the middle of the line.
		ProximityPointEdit test;
		int secondIndex = i + 1;
		if (secondIndex == (int)CHV.list.size())
		{
			secondIndex = 0;
		}
		test.x = (CHV.list[i].x + CHV.list[secondIndex].x) / 2.0f;
		test.z = (CHV.list[i].z + CHV.list[secondIndex].z) / 2.0f;
		float minDist = abs(sqrt(pow((test.x - CHV.list[i].x), 2.0f) + pow((test.z - CHV.list[i].z), 2.0f)));

		for (uint32_t j = 0; j < CHV.list.size(); j++)
		{
			float newDist = abs(sqrt(pow((test.x - CHV.list[j].x), 2.0f) + pow((test.z - CHV.list[j].z), 2.0f)));
			if ((minDist > newDist) && (minDist > 20.0f)) //Alter the minDist > X check to be the units the player can move per frame (Preferably a high number cause otherwise it's very expensive)
			{
				std::cout << "Added new point to fix edge case at: (" << test.x << ", " << test.z << ")" << std::endl;
				//Add the middle point to the list of points
				CHV.list.emplace(secondIndex, test);
				EdgeCaseFixer(CHV, lastIncluded);
				//Increase the currentSize and break.
				currentSize++;
				break;
			}
		}
	}
}
