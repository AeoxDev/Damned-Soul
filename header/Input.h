#pragma once

extern int m_keyInput[256];

enum MouseButton
{
	left,
	right,
	middle,
};

enum MouseButtonState
{
	noEvent,
	pressed,//Pressed down this frame
	released,//From down/pressed to up
};

enum MouseButtonDown
{
	up,//Not down
	down,//Not released yet
};

extern MouseButtonState mouseButtonPressed[3];//Left, right, middle
extern MouseButtonDown mouseButtonDown[3];
extern int mouseX;
extern int mouseY;

void GetInput();
void ResetMouse();