#include "Item/CItem_Crop.h"

ACItem_Crop::ACItem_Crop()
{
	SetType(EInteractObjectType::Crop);
}

bool ACItem_Crop::SetCropData(TOptional<FCropData> InDataOpt)
{
	if (InDataOpt.IsSet())
	{
		CropData  = InDataOpt.GetValue();
		return true;
	}
	return false;
}

bool ACItem_Crop::UseItem()
{
	return false;
}

FName ACItem_Crop::GetInteractName()
{
	return FName();
}
