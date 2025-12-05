#pragma once
#include "EntityID.h"
#include "UIComponents.h"

struct DSFLOAT2;

class UIEditorSystem
{
private:
	// non-copyable
	UIEditorSystem(const UIEditorSystem&) = delete;
	UIEditorSystem& operator=(const UIEditorSystem&) = delete;

public:
	UIEditorSystem() = default;
	~UIEditorSystem() = default;

	// Called every frame by the systems loop
	bool Update();

	// Enable/disable edit mode (Save on disable, Load on enable)
	static void SetEditMode(bool enabled);
	static bool GetEditMode();

	// Explicit save/load (also called automatically)
	static void SaveLayout(const char* filepath = "ui_layout.cfg");
	static void LoadLayout(const char* filepath = "ui_layout.cfg");

};