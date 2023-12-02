#include "ModelComponent.h"
#include "Model.h"

//int LoadModel(const char* filename)
//{
//    /*models.push_back(Model());
//    models[models.size() - 1].Load(filename);
//    return models.size() - 1;*/
//    return 0;
//}

void ModelSharedData::ResetTempColor()
{
	bcmR_temp = bcmG_temp = bcmB_temp = 1.f;
	bcaR_temp = bcaG_temp = bcaB_temp = 0.f;
}

float ModelSharedData::GetRedMult() const
{
	return bcmR_temp * colorMultiplicativeRed;
}

float ModelSharedData::GetGreenMult() const
{
	return bcmG_temp * colorMultiplicativeGreen;
}

float ModelSharedData::GetBlueMult() const
{
	return bcmB_temp * colorMultiplicativeBlue;
}

float ModelSharedData::GetRedAdd() const
{
	return bcaR_temp + colorAdditiveRed;
}

float ModelSharedData::GetGreenAdd() const
{
	return bcaG_temp + colorAdditiveGreen;
}

float ModelSharedData::GetBlueAdd() const
{
	return bcaB_temp + colorAdditiveBlue;
}