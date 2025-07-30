#include "Crops/CBase_Crop.h"
#include "Global.h"
#include "CGameModeBase.h"
#include "Farm/CFarmField.h"
#include "Components/CMoistureComponent.h"
#include "Components/CNutritionComponent.h"
#include "Components/CHealthComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "UI/CCropWidget.h"
#include "Item/CItem_Crop.h"
#include "CGameInstance.h"
#include "Item/CItem_Crop.h"
#include "Particles/ParticleSystem.h"
#include "Controller/CPlayerController.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY(LogCrop);

ACBase_Crop::ACBase_Crop()
	: OwnerField(nullptr)
{
	// RootComp;
	CHelpers::CreateSceneComponent(this, &RootComp, "RootComp", RootComponent);
	// Mesh
	CHelpers::CreateSceneComponent(this, &MeshComp, "MeshComp", RootComponent);
	
	// Box Component
	CHelpers::CreateSceneComponent(this, &BoxComp, "BoxComp", RootComponent);

	// Camera Component
	CHelpers::CreateSceneComponent(this, &CameraComp, "CameraComp", RootComponent);

	// Mosisture & Nutrition
	CHelpers::CreateActorComponent(this, &MoistureComp, "MoistureComp");
	CHelpers::CreateActorComponent(this, &NutritionComp, "NutritionComp");
	CHelpers::CreateActorComponent(this, &HealthComp, "HealthComp");

	// Set Box Component
	BoxComp->SetBoxExtent(FVector(64.0f, 64.0f, 96.0f));
	BoxComp->SetRelativeLocation(FVector(0.0f, 0.0f, 96.0f));
	BoxComp->SetCollisionProfileName(TEXT("ActionInteractObject"));
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Set Camera Component
	CameraComp->SetRelativeLocation(FVector(-135.0f, 0.0f, 200.0f));
	CameraComp->SetRelativeRotation(FQuat(FRotator(-30.0f, 0.0f, 0.0f)));

	// Property
	CurrentGrowLevel = 0;
	CurrentGrowValue = 0.0f;
	TargetGrowthValue = 0.0f;
	UpdateTime = 1.0f;

	SetType(EInteractObjectType::Crop);
}

void ACBase_Crop::BeginPlay()
{
	Super::BeginPlay();
	
	// Material
	UMaterialInterface* Material = MeshComp->GetMaterial(0);
	if (Material)
	{
		CropMaterial = UMaterialInstanceDynamic::Create(Material, this);
		MeshComp->SetMaterial(0, CropMaterial);
	}

	//HealthComp->OnStateValueChanged.AddDynamic(this, &ACBase_Crop::ChangeQualityByHealth);
	HealthComp->OnHealthStateChanged.AddDynamic(this, &ACBase_Crop::ChangeByHealthState);
	SetUnInteractable();
	GrowUp();
	SetAutoGrowTimer(UpdateTime, true, UpdateTime);
}

void ACBase_Crop::SetInteractable()
{
	bInteractable = true;
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACBase_Crop::SetUnInteractable()
{
	bInteractable = false;
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACBase_Crop::SetDelayedInteractable(float DelayTime)
{
	if (GetWorld()->GetTimerManager().TimerExists(InteractTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractTimer);
	}
	GetWorld()->GetTimerManager().SetTimer(InteractTimer, this, &ACBase_Crop::SetInteractable, DelayTime, false);
}

void ACBase_Crop::SetDelayedUninteractable(float DelayTime)
{
	if (GetWorld()->GetTimerManager().TimerExists(InteractTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractTimer);
	}
	GetWorld()->GetTimerManager().SetTimer(InteractTimer, this, &ACBase_Crop::SetUnInteractable, DelayTime, false);
}

void ACBase_Crop::SetType(EInteractObjectType InNewType)
{
	InteractType = InNewType;
}

void ACBase_Crop::Interact(AActor* OtherActor)
{
	if (IsHarvestable())
	{
		UE_LOG(LogCrop, Warning, TEXT("Crop harvested! CropName : %s"), *(CropName.ToString()));
		DoHarvest();
	}
	else if (IsDead())
	{
		OwnerField->EraseCrop();
		UE_LOG(LogCrop, Warning, TEXT("Crop is dead! CropName : %s"), *(CropName.ToString()));
		this->Destroy();
	}
}

bool ACBase_Crop::OnHovered()
{
	UE_LOG(LogCrop, Warning, TEXT("%s is Hovered!"), *GetInteractName().ToString());
	MeshComp->SetRenderCustomDepth(true);
	MeshComp->SetCustomDepthStencilValue(1);

	return false;
}

bool ACBase_Crop::OnUnhovered()
{
	UE_LOG(LogCrop, Warning, TEXT("%s is Unhovered!"), *GetInteractName().ToString());
	MeshComp->SetRenderCustomDepth(false);
	MeshComp->SetCustomDepthStencilValue(0);

	return false;
}

void ACBase_Crop::SetAutoGrowTimer(float InFirstDelay, bool InbLoop, float InLoopDelay)
{
	GetWorld()->GetTimerManager().ClearTimer(AutoGrowTimer);
	GetWorld()->GetTimerManager().SetTimer(AutoGrowTimer, this, &ACBase_Crop::AutoGrow, InLoopDelay, InbLoop, InFirstDelay);
}

void ACBase_Crop::GrowUp()
{
	const TOptional<FCropData>& CropDataOpt = GetCropData();
	if (CropDataOpt.IsSet())
	{
		FCropData CropData = CropDataOpt.GetValue();
		if (CurrentGrowLevel == 0)
		{
			CHelpers::GetAssetDynamic(&GrowthParticleEffect, CropData.GrowUpPraticleEffectRef);
			CHelpers::GetAssetDynamic(&GrowthSoundEffect, CropData.GrowUpSoundEffectRef);
		}
		//CheckTrue(CurrentGrowLevel == CropData.MaxLevel);

		PlayGrowthEffects();
		if (IsHarvestable())
		{
			SetInteractable();
		}

		++CurrentGrowLevel;
		CurrentGrowValue = 0.0f;
		SetCropDatas();

		if (CurrentGrowLevel == 1)
		{
			float MoistureValue = (MoistureComp->GetSafeRange().X + MoistureComp->GetSafeRange().Y) * 0.5f;
			MoistureComp->AddMoisture(MoistureValue);
			float NutritionValue = (NutritionComp->GetSafeRange().X + NutritionComp->GetSafeRange().Y) * 0.5f;
			NutritionComp->AddNutrition(NutritionValue);
		}
	}
	else
	{
		UE_LOG(LogCrop, Error, TEXT("GowthData doesn't set. %s : "), *CropName.ToString());
	}
}

void ACBase_Crop::PlayGrowthEffects()
{
	// 파티클 효과 재생
	if (GrowthParticleEffect)  // 파티클이 설정되어 있으면
	{
		UWorld* World = GetWorld();
		if (World)
		{
			// 현재 위치에 파티클을 재생
			UGameplayStatics::SpawnEmitterAtLocation(World, GrowthParticleEffect, GetActorLocation());
		}
	}

	// 사운드 효과 재생
	if (GrowthSoundEffect)  // 사운드가 설정되어 있으면
	{
		UWorld* World = GetWorld();
		if (World)
		{
			// 현재 위치에서 사운드를 재생
			UGameplayStatics::PlaySoundAtLocation(World, GrowthSoundEffect, GetActorLocation());
		}
	}
}

bool ACBase_Crop::IsDead()
{
	return HealthComp->IsDead();
}

bool ACBase_Crop::IsHarvestable()
{	
	const TOptional<FCropData>& CropDataOpt = GetCropData();
	if (CropDataOpt.IsSet())
	{
		FCropData CropData = CropDataOpt.GetValue();
		UE_LOG(LogCrop, Display, TEXT("Current GrowLevel : %d. MaxLevel : %d"), CurrentGrowLevel, CropData.MaxLevel);
		return CurrentGrowLevel == CropData.MaxLevel;
	}
	else
	{
		UE_LOG(LogCrop, Error, TEXT("GowthData doesn't set. %s : "), *CropName.ToString());
	}
	return false;
}

void ACBase_Crop::DoHarvest()
{
	if (IsDead())
	{
		UE_LOG(LogCrop, Error, TEXT("Crop is dead!. CropName : %s"), *CropName.ToString());

	}
	UWorld* World = GetWorld();
	if (World)
	{
		const TOptional<FCropData>& DataOpt = GetCropData();
		if (DataOpt.IsSet())
		{
			const FCropData& Data = DataOpt.GetValue();
			const TOptional<FItemAssetData>& ItemAssetDataOpt = GetCropItemData();
			if (ItemAssetDataOpt.IsSet())
			{
				const FItemAssetData& ItemAssetData = ItemAssetDataOpt.GetValue();
				TSubclassOf<ACItem_Crop> CropItemClass;
				CHelpers::GetClassDynamic(&CropItemClass, ItemAssetData.ItemClassRef);
				if (CropItemClass)
				{
					for (auto& SpawnPoint : SpawnPoints)
					{
						FTransform SpawnTarnsform = SpawnPoint * GetTransform();
						ACItem_Crop* CropItem = World->SpawnActorDeferred<ACItem_Crop>(CropItemClass, SpawnTarnsform);
						CropItem->SetAvailableCnt(1);
						if (!CropItem)
						{
							UE_LOG(LogCrop, Error, TEXT("Failed to spawn crop item actor."));
						}
						CropItem->FinishSpawning(SpawnTarnsform);
					}
				}
				else
				{
					UE_LOG(LogCrop, Error, TEXT("Creation of crop item failed. Invalid Clas Ref. %s"), *(UEnum::GetValueAsString(ItemAssetData.ItemID)));
				}
			}
			else
			{
				UE_LOG(LogItem, Error, TEXT("Can't find ItemData! CropName : %s, ItemId : %s"), *(CropName.ToString()), *(UEnum::GetValueAsString(Data.IDForQuality[0])));
			}
		}
	}
	OwnerField->EraseCrop();
	this->Destroy();
}

void ACBase_Crop::SetCropQuality(EQualityType InType)
{
	CropQuality = InType;
}

void ACBase_Crop::ChangeByHealthState(EHealthState InState)
{
	float LifeValue = 1.0f;
	switch (HealthComp->GetCurrentState())
	{
		case EHealthState::Healthy:
		{
			LifeValue = 1.0f;
			SetCropQuality(EQualityType::High);
		}
		break;
		case EHealthState::Sick:
		{
			LifeValue = 0.8f;
			SetCropQuality(EQualityType::Normal);
		}
		break;
		case EHealthState::Deadly:
		{
			LifeValue = 0.5f;
			SetCropQuality(EQualityType::Low);
		}
		break;
		case EHealthState::Dead:
		{
			LifeValue = 0.3f;
			ChangeDead();
		}
		break;
	}
	CropMaterial->SetScalarParameterValue(FName("Life"), LifeValue);
}

void ACBase_Crop::ChangeDead()
{
	// Interact
	SetInteractable();
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	// Stop Grow
	GetWorld()->GetTimerManager().ClearTimer(AutoGrowTimer);

	CropMaterial->SetScalarParameterValue(FName("Life"), 0.0f);
}

void ACBase_Crop::SetFarmField(ACFarmField* InFarmField)
{
	if (InFarmField)
	{
		OwnerField = InFarmField;
	}
}

void ACBase_Crop::AutoGrow()
{
	if (IsDead()) return;
	const TOptional<FCropGrowthData>& GrowthDataOpt = GetGrowthData();
	if (GrowthDataOpt.IsSet())
	{
		const FCropGrowthData& GrowthData = GrowthDataOpt.GetValue();
		if (GrowthData.CropName == CropName)
		{
			if (OwnerField)
			{
				// 양분 및 수분 소비
				float AvailableNutrition = FMath::Min(GrowthData.ConsumeNutrition, OwnerField->GetNutritionComp()->GetCurrentValue());
				NutritionComp->AddNutrition(AvailableNutrition);
				OwnerField->GetNutritionComp()->ReduceNutrition(AvailableNutrition);

				float AvailableMoisture = FMath::Min(GrowthData.ConsumeMoisture, OwnerField->GetMoistureComp()->GetCurrentValue());
				MoistureComp->AddMoisture(AvailableMoisture);
				OwnerField->GetMoistureComp()->ReduceMoisture(AvailableMoisture);

				// Drain Nutrition From Field
				float LeftNutritionapacity = GrowthData.Max_Nutrition - NutritionComp->GetCurrentValue();
				float CurrentConsumeNutrition = LeftNutritionapacity > GrowthData.ConsumeNutrition ? GrowthData.ConsumeMoisture : LeftNutritionapacity;

				float FieldNutrtion = OwnerField->GetNutritionComp()->GetCurrentValue();
				CurrentConsumeNutrition = FieldNutrtion < CurrentConsumeNutrition ? FieldNutrtion : CurrentConsumeNutrition;

				NutritionComp->AddNutrition(CurrentConsumeNutrition);
				OwnerField->GetNutritionComp()->ReduceNutrition(CurrentConsumeNutrition);

				// Drain Moisture From Field
				float LeftMoistureCapacity = GrowthData.Max_Moisture - MoistureComp->GetCurrentValue();
				float CurrentConsumeMoisture = LeftMoistureCapacity > GrowthData.ConsumeMoisture ? GrowthData.ConsumeMoisture : LeftMoistureCapacity;

				float FieldMoisture = OwnerField->GetMoistureComp()->GetCurrentValue();
				CurrentConsumeMoisture = FieldMoisture < CurrentConsumeMoisture ? FieldMoisture : CurrentConsumeMoisture;

				MoistureComp->AddMoisture(CurrentConsumeMoisture);
				OwnerField->GetMoistureComp()->ReduceMoisture(CurrentConsumeMoisture);

				// Change Health
				if (NutritionComp->IsEnough() && MoistureComp->IsEnough())
				{
					HealthComp->IncreaseHealth(0.05f);
				}
				else
				{
					HealthComp->DecreaseHealth(0.05f);
				}
			}

			// Grow
			float GrowUpValue = GrowthData.DefaultGrowUpValue;

			// Calc Weather, Moisture, Nutrition Effect

			// 성장 값 증가
			float PrevValue = CurrentGrowValue;
			CurrentGrowValue += GrowUpValue;
			if (CurrentGrowValue > GrowthData.TargetGrowthValue)
			{
				GrowUp();
				// TargetGrowthValue = GrowthData.TargetGrowthValue;
				//UE_LOG(LogTemp, Warning, TEXT("TargetGrowthValue : %f, CurrentGrowthValue : %f"), TargetGrowthValue, CurrentGrowValue);
			}
			OnGrowthChanged.Broadcast(PrevValue, CurrentGrowValue, GrowthData.TargetGrowthValue);
		}
	}
}

void ACBase_Crop::SetCropDatas()
{
	const TOptional<FCropGrowthData>& GrowthData = GetGrowthData();
	if (GrowthData.IsSet())
	{
		const FCropGrowthData& CurrentGrowthData = GrowthData.GetValue();

		UStaticMesh* MeshAsset;
		CHelpers::GetAssetDynamic(&MeshAsset, CurrentGrowthData.MeshRef);
		if (!IsValid(MeshAsset))
		{
			UE_LOG(LogCrop, Error, TEXT("Can't find Mesh Asset at %s"), *CurrentGrowthData.MeshRef);
		}
		else
		{
			MeshComp->SetStaticMesh(MeshAsset);
		}
		//MeshComp->SetStaticMesh(CropMeshes[CurrentGrowLevel]);
		NutritionComp->SetSafeRange(CurrentGrowthData.SafeRange_Nutrition);
		MoistureComp->SetSafeRange(CurrentGrowthData.SafeRange_Moisture);
		// Set Current Health as much as Max Health when first grow up.
		HealthComp->SetSafeRange(FVector2D(CurrentGrowthData.Max_Health * 0.3f, CurrentGrowthData.Max_Health * 0.7f));
		HealthComp->SetMaxHealth(CurrentGrowthData.Max_Health, CurrentGrowLevel == 1);
		TargetGrowthValue = CurrentGrowthData.TargetGrowthValue;
	}
	else
	{
		UE_LOG(LogCrop, Error, TEXT("GowthData doesn't set. %s : "), *CropName.ToString());
	}
	
}

const TOptional<FCropGrowthData> ACBase_Crop::GetGrowthData()
{
	UGameInstance* Instance = GetGameInstance();
	if (Instance)
	{
		UCGameInstance* MyInstance = Cast<UCGameInstance>(Instance);
		if (MyInstance)
		{
			return MyInstance->GetCropGrowthData(CropName, CurrentGrowLevel);
		}
	}
	else
	{
		UE_LOG(LogCrop, Error, TEXT("Can't get GameInstance. %s : "), *CropName.ToString());
	}
	return TOptional<FCropGrowthData>();
}

const TOptional<FCropData> ACBase_Crop::GetCropData()
{
	UGameInstance* Instance = GetGameInstance();
	if (Instance)
	{
		UCGameInstance* MyInstance = Cast<UCGameInstance>(Instance);
		if (MyInstance)
		{
			return MyInstance->GetCropDefaultData(CropName);
		}
	}
	else
	{
		UE_LOG(LogCrop, Error, TEXT("Can't get GameInstance. %s : "), *CropName.ToString());
	}
	return TOptional<FCropData>();
}

const TOptional<FItemAssetData> ACBase_Crop::GetCropItemData()
{
	UGameInstance* GI = GetGameInstance();
	if (UCGameInstance* GameInstance = Cast<UCGameInstance>(GI))
	{
		TOptional<FCropData> CropDataOpt = GameInstance->GetCropDefaultData(CropName);
		if (CropDataOpt.IsSet())
		{
			const FCropData& Data = CropDataOpt.GetValue();
			if (Data.CropName == CropName)
			{
				TOptional<FItemAssetData> ItemAssetDataOpt;
				switch (GetCropQuality())
				{
				case EQualityType::Low:
				{
					return GameInstance->GetItemtAssetData(Data.IDForQuality[0]);
				}
				break;
				case EQualityType::Normal:
				{
					return GameInstance->GetItemtAssetData(Data.IDForQuality[1]);
				}
				break;
				case EQualityType::High:
				{
					return GameInstance->GetItemtAssetData(Data.IDForQuality[2]);
				}
				break;
				default:
				{
					UE_LOG(LogCrop, Error, TEXT("Creation of crop item failed. Invalid Quality. %s"), *(UEnum::GetValueAsString(GetCropQuality())));
				}
				break;
				}
			}
			else 
			{
				UE_LOG(LogCrop, Error, TEXT("Can't find CropData! CropName : %s"), *(CropName.ToString()));
			}
		}
		else
		{
			UE_LOG(LogCrop, Error, TEXT("Can't find CropData! CropName : %s"), *(CropName.ToString()));
		}
	}
	return TOptional<FItemAssetData>();
}