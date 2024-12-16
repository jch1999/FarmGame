#include "Farm/CFarmField.h"
#include "Global.h"
#include "Components/CMoistureComponent.h"
#include "Components/CNutritionComponent.h"
#include "Components/CCultivationComponent.h"
#include "Crops/CBase_Crop.h"
#include "CGameModeBase.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UI/CFarmFieldWidget.h"

ACFarmField::ACFarmField()
{
	CHelpers::CreateSceneComponent(this, &MeshComp, "MeshComp", RootComponent);
	CHelpers::CreateActorComponent(this,&MoistureComp, "MoistureComp");
	CHelpers::CreateActorComponent(this, &NutritionComp, "NutritionComp");
	CHelpers::CreateActorComponent(this, &CultivationComp, "CultivationComp");

	UStaticMesh* MeshAsset;
	CHelpers::GetAsset(&MeshAsset, "/Game/Farm/Mesh/SM_FarmField");
	if (MeshAsset)
	{
		MeshComp->SetStaticMesh(MeshAsset);
	}
}

void ACFarmField::BeginPlay()
{
	Super::BeginPlay();
	
	MoistureComp->AddMoisture(30.0f);
	MoistureComp->SetAutoReduceAmount(0.1f);
	MoistureComp->SetAutoReduceTimer(1.0f, true, 1.0f);
	MoistureComp->SetSafeRange(FVector2D(0.0f, 100.0f));

	NutritionComp->AddNutrition(50.0f);
	NutritionComp->SetAutoReduceAmount(0.05f);
	NutritionComp->SetAutoReduceTimer(1.0f, true, 1.0f);
	NutritionComp->SetSafeRange(FVector2D(0.0f, 100.0f));

	CultivationComp->AddCultivation(25.0f);
	CultivationComp->SetAutoReduceAmount(0.5f);
	CultivationComp->SetAutoReduceTimer(1.0f, true, 1.0f);

	SetInteractable();

	// Set Dynamic Material
	FieldMaterial = UMaterialInstanceDynamic::Create(MeshComp->GetMaterial(0), nullptr);
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

	UCFarmFieldWidget* FarmFieldWidget = GameMode->GetFarmFieldWidget();
	CheckNull(FarmFieldWidget);

	CheckFalse(FarmFieldWidget->IsAvailable());

	FarmFieldWidget->SetFarmField(this);
	FarmFieldWidget->AddToViewport();

	SetUnInteractable();
}

bool ACFarmField::PlantCrop(TSubclassOf<ACBase_Crop> InCropClass, FTransform& InTM)
{
	CheckNullResult(InCropClass, false);
	CheckTrueResult(Crop != nullptr, false);

	Crop = GetWorld()->SpawnActorDeferred<ACBase_Crop>(InCropClass, InTM, this, nullptr);
	Crop->FinishSpawning(InTM);
	CheckNullResult(Crop, false);

	Crop->SetOwner(this);

	return true;
}
