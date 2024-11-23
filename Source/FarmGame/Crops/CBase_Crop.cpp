#include "Crops/CBase_Crop.h"
#include "Global.h"

ACBase_Crop::ACBase_Crop()
{
	// Mesh
	CHelpers::CreateSceneComponent(this, &MeshComp, "MeshComp", RootComponent);

	// Mosisture & Nutrition
	CHelpers::CreateActorComponent(this, &MoistureComp, "MoistureComp");
	CHelpers::CreateActorComponent(this, &NutritionComp, "NutritionComp");

	// Property
	GrowType = ECropGrowType::VegetativeGrowth;
}

void ACBase_Crop::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ACBase_Crop::IsInteractable()
{
	return bInteractable;
}

void ACBase_Crop::SetInteractable()
{
	bInteractable = true;
}

void ACBase_Crop::SetUnInteractable()
{
	bInteractable = false;
}

EInteractObjectType ACBase_Crop::GetType()
{
	return Type;
}

void ACBase_Crop::SetType(EInteractObjectType InNewType)
{
	Type = InNewType;
}

void ACBase_Crop::Interact()
{
}

FCropData ACBase_Crop::GetCurrentCropData()
{
	return Datas[(int32)GrowType];
}

void ACBase_Crop::Grow()
{
	GrowType = (ECropGrowType)((int32)GrowType + 1);
	CheckTrue(GrowType == ECropGrowType::MAX);
}

