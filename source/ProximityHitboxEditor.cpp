#include "ProximityHitboxEditor.h"

CurrentHitboxVariables CHV;

void HitboxEditorLoop(std::string& fileName)
{
	bool done = false;

	//Read hitbox file if it exists

	while (!done)
	{
		//Render the entire level as a top down view

		//Render the placed out points

		//Get click event and mouse position
			//If right click && within a point's radius --> Remove selected point.
			//Else if right click && not within a point's radius --> Remove last point in the list.

			//If left click && within a point's radius --> Move selected point.
			//Else if left click && not within a point's radius --> Place out a new point.

		//If *Selected Key* is pressed handle file creation/editing depending on fileName
			//Put done to true.
		
		//If *Selected Key* is pressed handle file removal
			//Put done to true.
	}
}
