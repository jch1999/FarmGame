#include "Crops/CBase_Crop.h"
#include "Global.h"
#include "CGameModeBase.h"
#include "Farm/CFarmField.h"
#include "Components/CMoistureComponent.h"
#include "Components/CNutritionComponent.h"
#include "Components/CHealthComponent.h"
#include "UI/CCropWidget.h"

ACBase_Crop::ACBase_Crop()
	: OwnerField(nullptr)
{
	// Mesh
	CHelpers::CreateSceneComponent(this, &MeshComp, "MeshComp", RootComponent);

	// Mosisture & Nutrition
	CHelpers::CreateActorComponent(this, &MoistureComp, "MoistureComp");
	CHelpers::CreateActorComponent(this, &NutritionComp, "NutritionComp");
	CHelpers::CreateActorComponent(this, &HealthComp, "HealthComp");

	// DataAsset
	CHelpers::GetAsset(&CropDataTable, "/Game/Datas/DT_CropDatas");

	// Property
	NowGrowLevel = 0;
	NowGrowValue = 0.0f;
	UpdateTime = 1.0f;

	// Set Components
	/*if (Datas.Num() > 0)
	{
		MeshComp->SetStaticMesh(CropMeshes[0]);
		MoistureComp->SetAutoReduceTimer(UpdateTime, true, UpdateTime);
		MoistureComp->AddMoisture(MoistureComp->GetSafeRange().Y);
		NutritionComp->SetAutoReduceTimer(UpdateTime, true, UpdateTime);
		NutritionComp->AddNutrition(NutritionComp->GetSafeRange().Y);
		HealthComp->SetMaxHealth(GetCurrentCropData().Max_Health, true);
		SetAutoGrowTimer(UpdateTime, true, UpdateTime);
	}*/

	// Load Data
	LoadCropData();

	SetType(EInteractObjectType::Crop);
}

void ACBase_Crop::BeginPlay()
{
	Super::BeginPlay();

	SetInteractable();
}

void ACBase_Crop::SetInteractable()
{
	bInteractable = true;
}

void ACBase_Crop::SetUnInteractable()
{
	bInteractable = false;
}

void ACBase_Crop::SetType(EInteractObjectType InNewType)
{
	InteractType = InNewType;
}

void ACBase_Crop::Interact(AActor* OtherActor)
{
	/*CheckFalse(bInteractable);

	ACGameModeBase* GameMode = Cast<ACGameModeBase>(GetWorld()->GetAuthGameMode());
	CheckNull(GameMode);

	UCCropWidget* CropWidget = GameMode->GetCropWidget();
	CheckNull(CropWidget);

	CheckFalse(CropWidget->IsAvailable());

	CropWidget->SetCrop(this);
	CropWidget->AddToViewport();*/

	SetUnInteractable();
}

bool ACBase_Crop::OnHovered()
{
	UE_LOG(LogTemp, Warning, TEXT("%s is Hovered!"), *GetInteractName().ToString());
	return false;
}

bool ACBase_Crop::OnUnhovered()
{
	UE_LOG(LogTemp, Warning, TEXT("%s is Unhovered!"), *GetInteractName().ToString());
	return false;
}

void ACBase_Crop::SetAutoGrowTimer(float InFirstDelay, bool InbLoop, float InLoopDelay)
{
	GetWorld()->GetTimerManager().ClearTimer(AutoGrowTimer);
	GetWorld()->GetTimerManager().SetTimer(AutoGrowTimer, this, &ACBase_Crop::AutoGrow, InLoopDelay, InbLoop, InFirstDelay);
}

void ACBase_Crop::GrowUp()
{
	CheckTrue(NowGrowLevel == MaxGrowLevel);

	++NowGrowLevel;
	if (CropData->GrowthStages.IsValidIndex(NowGrowLevel))
	{
		UStaticMesh* MeshAsset;
		CHelpers::GetAsset(&MeshAsset, CropData->GrowthStages[NowGrowLevel].MeshRef);
		MeshComp->SetStaticMesh(MeshAsset);
	}
	MeshComp->SetStaticMesh(CropMeshes[NowGrowLevel]);
	NutritionComp->SetSafeRange(GetCurrentGrowthData().SafeRange_Nutrition);
	MoistureComp->SetSafeRange(GetCurrentGrowthData().SafeRange_Moisture);
	HealthComp->SetMaxHealth(GetCurrentGrowthData().Max_Health, true);
}

void ACBase_Crop::AutoGrow()
{
	CheckNull(CropData);
	CheckNull(OwnerField);

	const FCropGrowthData& GrowthData = GetCurrentGrowthData();
	// 양분 및 수분 소비
	float AvailableNutrition = FMath::Min(GrowthData.ConsumeNutrition, OwnerField->GetNutritionComp()->GetCurrentNutrition());
	NutritionComp->AddNutrition(AvailableNutrition);
	OwnerField->GetNutritionComp()->ReduceNutrition(AvailableNutrition);

	float AvailableMoisture = FMath::Min(GrowthData.ConsumeMoisture, OwnerField->GetMoistureComp()->GetCurrentMoisture());
	MoistureComp->AddMoisture(AvailableMoisture);
	OwnerField->GetMoistureComp()->ReduceMoisture(AvailableMoisture);

	// 성장 값 증가
	NowGrowValue += GrowthData.DefaultGrowUpValue;
	if (NowGrowValue >= GrowthData.TargetGrowthValue)
	{
		GrowUp();
	}

	// Drain Nutrition From Field
	float LeftNutritionapacity = GetCurrentGrowthData().Max_Nutrition - NutritionComp->GetCurrentNutrition();
	float NowConsumeNutrition = LeftNutritionapacity > GetCurrentGrowthData().ConsumeNutrition ? GetCurrentGrowthData().ConsumeMoisture : LeftNutritionapacity;

	float FieldNutrtion = OwnerField->GetNutritionComp()->GetCurrentNutrition();
	NowConsumeNutrition = FieldNutrtion < NowConsumeNutrition ? FieldNutrtion : NowConsumeNutrition;
	
	NutritionComp->AddNutrition(NowConsumeNutrition);
	OwnerField->GetNutritionComp()->ReduceNutrition(NowConsumeNutrition);

	// Drain Moisture From Field
	float LeftMoistureCapacity = GetCurrentGrowthData().Max_Moisture - MoistureComp->GetCurrentMoisture();
	float NowConsumeMoisture = LeftMoistureCapacity > GetCurrentGrowthData().ConsumeMoisture ? GetCurrentGrowthData().ConsumeMoisture : LeftMoistureCapacity;

	float FieldMoisture = OwnerField->GetMoistureComp()->GetCurrentMoisture();
	NowConsumeMoisture = FieldMoisture < NowConsumeMoisture ? FieldMoisture : NowConsumeMoisture;

	MoistureComp->AddMoisture(NowConsumeMoisture);
	OwnerField->GetMoistureComp()->ReduceMoisture(NowConsumeMoisture);

	// Grow
	float GrowUpValue = GetCurrentGrowthData().DefaultGrowUpValue;

	// Calc Weather, Moisture, Nutrition Effect

	NowGrowValue += GrowUpValue;
	if (NowGrowValue > GetCurrentGrowthData().TargetGrowthValue)
	{
		GrowUp();
	}
}

void ACBase_Crop::LoadCropData()
{
	// Find Crop's Data form DataTable
	//if (CropDataTable)
	//{
	//	CropData = CropDataTable->FindRow<FCropData>(CropName, TEXT("Lookup Crop Data"));

	//	if (CropData)
	//	{
	//		MaxGrowLevel = CropData->MaxLevel;
	//		NowGrowLevel = 0;

	//		// 초기 성장 단계 Mesh 설정
	//		if (CropData->GrowthStages.IsValidIndex(NowGrowLevel))
	//		{
	//			UStaticMesh* MeshAsset;
	//			CHelpers::GetAsset(&MeshAsset, CropData->GrowthStages[NowGrowLevel].MeshRef);
	//			MeshComp->SetStaticMesh(MeshAsset);
	//		}
	//	}
	//}
}

