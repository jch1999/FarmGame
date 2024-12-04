#include "Crops/CBase_Crop.h"
#include "Global.h"
#include "CFarmField.h"
#include "Components/CMoistureComponent.h"
#include "Components/CNutritionComponent.h"

ACBase_Crop::ACBase_Crop()
{
	// Mesh
	CHelpers::CreateSceneComponent(this, &MeshComp, "MeshComp", RootComponent);

	// Mosisture & Nutrition
	CHelpers::CreateActorComponent(this, &MoistureComp, "MoistureComp");
	CHelpers::CreateActorComponent(this, &NutritionComp, "NutritionComp");

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
		NutritionComp->SetAutoReduceTimer(GetCurrentCropData().ReduceDelay_Nutrition, true, GetCurrentCropData().ReduceDelay_Nutrition);
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
	NutritionComp->SetSafeRange(Datas[NowGrowLevel].SafeRange_Nutrition);
	MoistureComp->SetSafeRange(Datas[NowGrowLevel].SafeRange_Moisture);
}

void ACBase_Crop::SetOwnerField(ACFarmField* InOwnerField)
{
	OwnerField = InOwnerField;
}

void ACBase_Crop::AutoGrow()
{
	CheckNull(OwnerField);

	// Drain Nutrition From Field
	if (NutritionComp->GetNutritionValue() < NutritionComp->GetSafeRange().Y)
	{
		UCNutritionComponent* FieldNutrtitionComp = CHelpers::GetComponent<UCNutritionComponent>(OwnerField);
		float NowConsumeNutrition = NutritionComp->GetSafeRange().Y - NutritionComp->GetNutritionValue();
		NowConsumeNutrition = FMath::Min
		(
			NowConsumeNutrition, 
			FieldNutrtitionComp->GetNutritionValue() >= Datas[NowGrowLevel].ConsumeNutrition
			? Datas[NowGrowLevel].ConsumeNutrition
			: FieldNutrtitionComp->GetNutritionValue()
		);
		NutritionComp->AddNutrition(NowConsumeNutrition);
		FieldNutrtitionComp->ReduceNutrition(NowConsumeNutrition);
	}

	// Drain Moisture From Field
	UCMoistureComponent* FieldMoistureComp = CHelpers::GetComponent<UCMoistureComponent>(OwnerField);
	float NowConsumeMoisture = FieldMoistureComp->GetMoistureValue() >= Datas[NowGrowLevel].ConsumeNutrition
		? Datas[NowGrowLevel].ConsumeMoisture
		: FieldMoistureComp->GetMoistureValue();
	MoistureComp->AddMoisture(NowConsumeMoisture);
	FieldMoistureComp->ReduceMoisture(NowConsumeMoisture);

	// Grow
	float GrowUpValue = GetCurrentCropData().DefaultGrowUpValue;

	// Calc Weather, Moisture, Nutrition Effect

	NowGrowValue += GrowUpValue;
	if (NowGrowValue > GetCurrentCropData().TargetGrowthValue)
	{
		GrowUp();
	}
}

