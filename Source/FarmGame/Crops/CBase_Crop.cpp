#include "Crops/CBase_Crop.h"
#include "Global.h"
#include "CFarmField.h"

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
		SetCropData();
	}

	// Property
	NowGrowLevel = 0;
	NowGrowValue = 0.0f;
}

void ACBase_Crop::BeginPlay()
{
	Super::BeginPlay();

	MoistureComp->SetAutoReduceTimer(GetCurrentCropData().ReduceDelay_M, true, GetCurrentCropData().ReduceDelay_M);
	NutritionComp->SetAutoReduceTimer(GetCurrentCropData().ReduceDelay_N, true, GetCurrentCropData().ReduceDelay_N);
	SetAutoGrowTimer(Datas[0].GrowDelay, true, Datas[0].GrowDelay);
}

void ACBase_Crop::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (!CropData && !IsRunningGame())
	{
		SetCropData();
	}
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
	/*GrowType = (ECropGrowType)((int32)GrowType + 1);
	CheckTrue(GrowType == ECropGrowType::MAX);*/
	CheckTrue(NowGrowLevel == MaxGrowLevel);
}

void ACBase_Crop::SetOwnerField(ACFarmField* InOwnerField)
{
	OwnerField = InOwnerField;
}

void ACBase_Crop::SetCropData()
{
	TArray<FCropData*> CropDatas;
	CropData->GetAllRows(TEXT("Fetching all rows"), CropDatas);
	
	for (const auto& Data : CropDatas)
	{
		if (Data->CropName == CropName)
		{
			Datas.Add(*Data);
		}
	}

	CropMeshes.SetNum(Datas.Num());
	for (int32 i = 0; i< CropMeshes.Num(); ++i)
	{
		CHelpers::GetAssetDynamic(&CropMeshes[i], Datas[i].MeshRef);
	}

	MeshComp->SetStaticMesh(CropMeshes[0]);
	NutritionComp->SetSafeRange(Datas[0].SafeRange_N);
	MoistureComp->SetSafeRange(Datas[0].SafeRange_M);
}

void ACBase_Crop::AutoGrow()
{
	CheckNull(OwnerField);

	// Drain Nutrition From Field
	// Nutrition has Max Limit.
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
	// Moisture dosen't have Max Limit.
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

