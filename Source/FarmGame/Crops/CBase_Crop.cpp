#include "Crops/CBase_Crop.h"
#include "Global.h"
#include "CFarmField.h"
#include "Components/CMoistureComponent.h"
#include "Components/CNutritionComponent.h"
#include "Components/CHealthComponent.h"

ACBase_Crop::ACBase_Crop()
{
	// Mesh
	CHelpers::CreateSceneComponent(this, &MeshComp, "MeshComp", RootComponent);

	// Mosisture & Nutrition
	CHelpers::CreateActorComponent(this, &MoistureComp, "MoistureComp");
	CHelpers::CreateActorComponent(this, &NutritionComp, "NutritionComp");
	CHelpers::CreateActorComponent(this, &HealthComp, "HealthComp");

	// DataAsset
	CHelpers::GetAsset(&CropData, "/Game/Datas/DT_CropDatas");

	if (CropData)
	{
		TArray<FCropData*> CropDatas;
		CropData->GetAllRows(TEXT("Fetching all rows"), CropDatas);

		for (const auto& Data : CropDatas)
		{
			if (Data->CropName == CropName)
			{
				Datas.Add(*Data);
				UStaticMesh* MeshAsset;
				CLog::Log(Data->MeshRef);
				CHelpers::GetAsset(&MeshAsset, Data->MeshRef);
				CropMeshes.Add(MeshAsset);
			}
		}
	}

	// Property
	NowGrowLevel = 0;
	NowGrowValue = 0.0f;

	// Set Components
	if (Datas.Num() > 0)
	{
		MeshComp->SetStaticMesh(CropMeshes[0]);
		MoistureComp->SetAutoReduceTimer(GetCurrentCropData().ReduceDelay_Moisture, true, GetCurrentCropData().ReduceDelay_Moisture);
		MoistureComp->AddMoisture(MoistureComp->GetSafeRange().Y);
		NutritionComp->SetAutoReduceTimer(GetCurrentCropData().ReduceDelay_Nutrition, true, GetCurrentCropData().ReduceDelay_Nutrition);
		NutritionComp->AddNutrition(NutritionComp->GetSafeRange().Y);
		HealthComp->SetMaxHealth(GetCurrentCropData().Max_Health, true);
		SetAutoGrowTimer(Datas[0].GrowDelay, true, Datas[0].GrowDelay);
	}
}

void ACBase_Crop::BeginPlay()
{
	Super::BeginPlay();
}

void ACBase_Crop::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	
}
	

bool ACBase_Crop::IsInteractable()
{
	return false;
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
	Type = InNewType;
}

void ACBase_Crop::Interact()
{
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
	MeshComp->SetStaticMesh(CropMeshes[NowGrowLevel]);
	NutritionComp->SetSafeRange(GetCurrentCropData().SafeRange_Nutrition);
	MoistureComp->SetSafeRange(GetCurrentCropData().SafeRange_Moisture);
	HealthComp->SetMaxHealth(GetCurrentCropData().Max_Health, true);
}

void ACBase_Crop::SetOwnerField(ACFarmField* InOwnerField)
{
	OwnerField = InOwnerField;
}

void ACBase_Crop::AutoGrow()
{
	CheckNull(OwnerField);

	// Drain Nutrition From Field
	float LeftNutritionapacity = GetCurrentCropData().Max_Nutrition - NutritionComp->GetNutritionValue();
	float NowConsumeNutrition = LeftNutritionapacity > GetCurrentCropData().ConsumeNutrition ? GetCurrentCropData().ConsumeMoisture : LeftNutritionapacity;

	UCNutritionComponent* FieldNutrtitionComp = CHelpers::GetComponent<UCNutritionComponent>(OwnerField);
	float FieldNutrtion = FieldNutrtitionComp->GetNutritionValue();
	NowConsumeNutrition = FieldNutrtion < NowConsumeNutrition ? FieldNutrtion : NowConsumeNutrition;
	
	NutritionComp->AddNutrition(NowConsumeNutrition);
	FieldNutrtitionComp->ReduceNutrition(NowConsumeNutrition);

	// Drain Moisture From Field
	float LeftMoistureCapacity = GetCurrentCropData().Max_Moisture - MoistureComp->GetMoistureValue();
	float NowConsumeMoisture = LeftMoistureCapacity > GetCurrentCropData().ConsumeMoisture ? GetCurrentCropData().ConsumeMoisture : LeftMoistureCapacity;

	UCMoistureComponent* FieldMoisutreComp = CHelpers::GetComponent<UCMoistureComponent>(OwnerField);
	float FieldMoisture = FieldMoisutreComp->GetMoistureValue();
	NowConsumeMoisture = FieldMoisture < NowConsumeMoisture ? FieldMoisture : NowConsumeMoisture;

	MoistureComp->AddMoisture(NowConsumeMoisture);
	FieldMoisutreComp->ReduceMoisture(NowConsumeMoisture);

	// Grow
	float GrowUpValue = GetCurrentCropData().DefaultGrowUpValue;

	// Calc Weather, Moisture, Nutrition Effect

	NowGrowValue += GrowUpValue;
	if (NowGrowValue > GetCurrentCropData().TargetGrowthValue)
	{
		GrowUp();
	}
}

