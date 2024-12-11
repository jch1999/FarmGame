#include "Crops/CFarmField.h"
#include "Global.h"
#include "Components/CMoistureComponent.h"
#include "Components/CNutritionComponent.h"
#include "Components/CCultivationComponent.h"
#include "CBase_Crop.h"
#include "CGameModeBase.h"

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
	
	MoistureComp->AddMoisture(30.0f);
	MoistureComp->SetAutoReduceAmount(0.05f);
	MoistureComp->SetAutoReduceTimer(1.0f, true, 1.0f);

	NutritionComp->AddNutrition(50.0f);
	NutritionComp->SetAutoReduceAmount(0.05f);
	NutritionComp->SetAutoReduceTimer(1.0f, true, 1.0f);

	CultivationComp->AddCultivation(25.0f);
	CultivationComp->SetAutoReduceAmount(5.0f);
	CultivationComp->SetAutoReduceTimer(10.0f, true, 10.0f);
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
	ACGameModeBase* GameMode = Cast<ACGameModeBase>(GetWorld()->GetAuthGameMode());
	CheckNull(GameMode);

	
	SetUnInteractable();
}

bool ACFarmField::PlantCrop(TSubclassOf<ACBase_Crop> InCropClass, FTransform& InTM)
{
	CheckNullResult(InCropClass, false);
	CheckTrueResult(Crop != nullptr, false);

	Crop = GetWorld()->SpawnActorDeferred<ACBase_Crop>(InCropClass, InTM, this, nullptr);
	Crop->FinishSpawning(InTM);
	CheckNullResult(Crop, false);

	Crop->SetOwnerField(this);

	return true;
}
