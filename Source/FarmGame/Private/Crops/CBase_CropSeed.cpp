#include "Crops/CBase_CropSeed.h"
#include "Crops/CBase_Crop.h"

ACBase_CropSeed::ACBase_CropSeed()
{
}

bool ACBase_CropSeed::IsUsable() const
{
	return bUsable;
}

void ACBase_CropSeed::SetUsable()
{
	bUsable = true;
}

void ACBase_CropSeed::SetUnsable()
{
	bUsable = false;
}

bool ACBase_CropSeed::UseItem()
{
	return false;
}

void ACBase_CropSeed::AddAvailableCount(int32 InAmount)
{
	AvailableCount += InAmount;
}

void ACBase_CropSeed::ReduceAvailableCount(int32 InAmount)
{
	AvailableCount -= InAmount;
}

int32 ACBase_CropSeed::GetAvailableCount() const
{
	return AvailableCount;
}

