#include "Crops/CFarmField.h"
#include "Global.h"
#include "Components/CMoistureComponent.h"
#include "Components/CNutritionComponent.h"
#include "Components/CCultivationComponent.h"
#include "CBase_Crop.h"

ACFarmField::ACFarmField()
{
	CHelpers::CreateSceneComponent(this, &MeshComp, "MeshComp", RootComponent);
	CHelpers::CreateActorComponent(this,&MoistureComp, "MoistureComp");
	CHelpers::CreateActorComponent(this, &NutritionComp, "NutritionComp");
	CHelpers::CreateActorComponent(this, &CultivationComp, "CultivationComp");

	UStaticMesh* MeshAsset;
	CHelpers::GetAsset(&MeshAsset, "/Game/CroupOutAssets/Environment/Meshes/Crops/SM_Crop_Corn_04");
	if (MeshAsset)
	{
		MeshComp->SetStaticMesh(MeshAsset);
	}
}

void ACFarmField::BeginPlay()
{
	Super::BeginPlay();
	
	//MoistureComp->SetAutoReduceTimer()
	//NutritionComp->SetAutoReduceTimer()
	//CultivationComp->SetAutoReduceTimer()
}

bool ACFarmField::IsInteractable()
{
	return bInteractable;
}

void ACFarmField::SetInteractable()
{
	bInteractable = true;
}

void ACFarmField::SetUnInteractable()
{
	bInteractable = false;
}

EInteractObjectType ACFarmField::GetType()
{
	return Type;
}

void ACFarmField::SetType(EInteractObjectType InNewType)
{
	Type = InNewType;
}

void ACFarmField::Interact()
{
}
