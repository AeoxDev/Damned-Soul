#include "StaticHazardComponent.h"
#include "D3D11Helper.h"
#include "Model.h"
#include <string>
void StaticHazardTextureComponent::LoadPlaneAndTexture(char* texture, char* model)
{
	std::string adress = texture;
	adress = "HazardMap\\" + adress;
	//Create a plane and load a texture
	textureID = LoadTexture(adress.c_str());
	//modelID = LoadModel();//Load plane
}
