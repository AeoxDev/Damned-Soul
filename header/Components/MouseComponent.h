#pragma once

struct EntityID;
struct MouseComponent;

void MouseComponentAddComponent(EntityID entityID);
void MouseComponentUpdateDirection(EntityID entityID, const bool& isPerspective = true);

float MouseComponentGetDirectionX(MouseComponent*& mouseComponent);
float MouseComponentGetDirectionZ(MouseComponent*& mouseComponent);