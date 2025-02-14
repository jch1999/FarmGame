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
	//CHelpers::GetAsset(&CropDataTable, "/Game/Datas/DT_CropDatas");

	// Property
	CurrentGrowLevel = 0;
	CurrentGrowValue = 0.0f;
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

	SetType(EInteractObjectType::Crop);
}

void ACBase_Crop::BeginPlay()
{
	Super::BeginPlay();

	SetInteractable();
	GrowUp();
	SetAutoGrowTimer(UpdateTime, true, UpdateTime);

	// Component Setting
	MoistureComp->AddMoisture(MoistureComp->GetSafeRange().Y);
	NutritionComp->SetAutoReduceTimer(UpdateTime, true, UpdateTime);
	NutritionComp->AddNutrition(NutritionComp->GetSafeRange().Y);

	if (const TOptional<FCropGrowthData>& GrowthDataOpt_Start = GetCropGrowthData(CropName, CurrentGrowLevel))
	{
		if (GrowthDataOpt_Start.IsSet())
		{
			HealthComp->SetMaxHealth(GrowthDataOpt_Start.GetValue().Max_Health, true);
		}
	}
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

const TOptional<FCropData> ACBase_Crop::GetCropDefaultData(FName InCropName)
{
	static TMap<FName, FCropData> CropDefaultDataMap;

	// Exception handling
	if (!CropDefaultTable)
	{
		UE_LOG(LogTemp, Error, TEXT("Data Table is missing. Crop: %s"), *InCropName.ToString());
		return {};
	}

	if (CropDefaultDataMap.Contains(InCropName))
	{
		return CropDefaultDataMap[InCropName];
	}

	FCropData* NewData=CropDefaultTable->FindRow<FCropData>(InCropName, "Lookup CropDefaultData");
	if (NewData)
	{
		CropDefaultDataMap.Add(InCropName, *NewData);
		return CropDefaultDataMap[InCropName];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't Find Data about %s"), *InCropName.ToString());
		return {};
	}
}

const TOptional<FCropGrowthData> ACBase_Crop::GetCropGrowthData(FName InCropName, int32 InLevel)
{
	static TMap<FName, TArray<FCropGrowthData>> CropGrowthDataMap;

	// Exception handling
	if (!CropGrowthTable || !CropDefaultTable)
	{
		UE_LOG(LogTemp, Error, TEXT("Data Table is missing. Crop: %s"), *InCropName.ToString());
		return {};
	}

	TOptional<FCropData> CropDataOpt = GetCropDefaultData(InCropName);
	if (!CropDataOpt.IsSet())
	{
		return {};
	}
	const FCropData& CropData = CropDataOpt.GetValue();
	if (InLevel<1 || InLevel > CropData.MaxLevel)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Growth Level (%d) for Crop: %s"), InLevel, *InCropName.ToString());
		return {};
	}

	TArray<FCropGrowthData>* FoundArray = CropGrowthDataMap.Find(InCropName);
	if(CropGrowthDataMap.Contains(InCropName))
	{
		return CropGrowthDataMap[InCropName][InLevel - 1];
	}

	TArray<FCropGrowthData*> Rows;
	CropGrowthTable->GetAllRows(TEXT("Fetching all rows"), Rows);

	TArray<FCropGrowthData> NewRows;
	for (const auto& Data : Rows)
	{
		if (Data->CropName == CropName)
		{
			NewRows.Add(*Data);
		}
	}

	if (NewRows.Num() > 0)
	{
		CropGrowthDataMap.Add(InCropName, NewRows);
		return CropGrowthDataMap[InCropName][InLevel - 1];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't Find Data about %s"), *InCropName.ToString());
		return {};
	}
}

void ACBase_Crop::SetAutoGrowTimer(float InFirstDelay, bool InbLoop, float InLoopDelay)
{
	GetWorld()->GetTimerManager().ClearTimer(AutoGrowTimer);
	GetWorld()->GetTimerManager().SetTimer(AutoGrowTimer, this, &ACBase_Crop::AutoGrow, InLoopDelay, InbLoop, InFirstDelay);
}

void ACBase_Crop::GrowUp()
{
	if (const TOptional<FCropData>& CropDataOpt = GetCropDefaultData(CropName))
	{
		if (!CropDataOpt.IsSet())
		{
			return;
		}
		else
		{
			const FCropData& CropData = CropDataOpt.GetValue();
			CheckTrue(CurrentGrowLevel == CropData.MaxLevel);
		}
	}
	

	++CurrentGrowLevel;
	
	SetCropDatas();
}

bool ACBase_Crop::IsHarvestable()
{
	const TOptional<FCropData>& CropDataOpt = GetCropDefaultData(CropName);
	if (CropDataOpt.IsSet())
	{
		const FCropData& CropData = CropDataOpt.GetValue();
		return CurrentGrowLevel == CropData.MaxLevel;
	}
	return false;
}

void ACBase_Crop::AutoGrow()
{
	//CheckNull(CropData);
	CheckNull(OwnerField);

	const TOptional<FCropGrowthData>& GrowthDataOpt = GetCropGrowthData(CropName,CurrentGrowLevel);
	if (!GrowthDataOpt.IsSet()) return;

	const FCropGrowthData& GrowthData = GrowthDataOpt.GetValue();
	// 양분 및 수분 소비
	float AvailableNutrition = FMath::Min(GrowthData.ConsumeNutrition, OwnerField->GetNutritionComp()->GetCurrentNutrition());
	NutritionComp->AddNutrition(AvailableNutrition);
	OwnerField->GetNutritionComp()->ReduceNutrition(AvailableNutrition);

	float AvailableMoisture = FMath::Min(GrowthData.ConsumeMoisture, OwnerField->GetMoistureComp()->GetCurrentMoisture());
	MoistureComp->AddMoisture(AvailableMoisture);
	OwnerField->GetMoistureComp()->ReduceMoisture(AvailableMoisture);

	// Drain Nutrition From Field
	float LeftNutritionapacity = GrowthData.Max_Nutrition - NutritionComp->GetCurrentNutrition();
	float CurrentConsumeNutrition = LeftNutritionapacity > GrowthData.ConsumeNutrition ? GrowthData.ConsumeMoisture : LeftNutritionapacity;

	float FieldNutrtion = OwnerField->GetNutritionComp()->GetCurrentNutrition();
	CurrentConsumeNutrition = FieldNutrtion < CurrentConsumeNutrition ? FieldNutrtion : CurrentConsumeNutrition;
	
	NutritionComp->AddNutrition(CurrentConsumeNutrition);
	OwnerField->GetNutritionComp()->ReduceNutrition(CurrentConsumeNutrition);

	// Drain Moisture From Field
	float LeftMoistureCapacity = GrowthData.Max_Moisture - MoistureComp->GetCurrentMoisture();
	float CurrentConsumeMoisture = LeftMoistureCapacity > GrowthData.ConsumeMoisture ? GrowthData.ConsumeMoisture : LeftMoistureCapacity;

	float FieldMoisture = OwnerField->GetMoistureComp()->GetCurrentMoisture();
	CurrentConsumeMoisture = FieldMoisture < CurrentConsumeMoisture ? FieldMoisture : CurrentConsumeMoisture;

	MoistureComp->AddMoisture(CurrentConsumeMoisture);
	OwnerField->GetMoistureComp()->ReduceMoisture(CurrentConsumeMoisture);

	// Grow
	float GrowUpValue = GrowthData.DefaultGrowUpValue;

	// Calc Weather, Moisture, Nutrition Effect

	// 성장 값 증가
	CurrentGrowValue += GrowUpValue;
	if (CurrentGrowValue > GrowthData.TargetGrowthValue)
	{
		GrowUp();
	}
}

void ACBase_Crop::SetCropDatas()
{
	const TOptional<FCropGrowthData>& GrowthDataOpt = GetCropGrowthData(CropName, CurrentGrowLevel);
	if (!GrowthDataOpt.IsSet()) return;

	const FCropGrowthData& CurrentGrowthData = GrowthDataOpt.GetValue();
	
	UStaticMesh* MeshAsset;
	
	CHelpers::GetAsset(&MeshAsset, CurrentGrowthData.MeshRef);
	MeshComp->SetStaticMesh(MeshAsset);
	//MeshComp->SetStaticMesh(CropMeshes[CurrentGrowLevel]);
	NutritionComp->SetSafeRange(CurrentGrowthData.SafeRange_Nutrition);
	MoistureComp->SetSafeRange(CurrentGrowthData.SafeRange_Moisture);
	HealthComp->SetMaxHealth(CurrentGrowthData.Max_Health, true);
}

