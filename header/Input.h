#pragma once

enum ButtonState;



extern int keyInput[256];
extern ButtonState keyState[256];

enum MouseButton
{
	left,
	right,
	middle,
};

enum ButtonState
{
	noEvent,
	pressed,//Pressed down this frame
	released,//From down/pressed to up
};

enum ButtonDown
{
	up,//Not down
	down,//Not released yet
};

extern ButtonState mouseButtonPressed[3];//Left, right, middle
extern ButtonDown mouseButtonDown[3];
extern int mouseX;
extern int mouseY;

void GetInput();
void ResetInput();