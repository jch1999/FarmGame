#include "Crops/CBase_Crop.h"
#include "Global.h"
#include "CGameModeBase.h"
#include "Farm/CFarmField.h"
#include "Components/CMoistureComponent.h"
#include "Components/CNutritionComponent.h"
#include "Components/CHealthComponent.h"
#include "Components/BoxComponent.h"
#include "UI/CCropWidget.h"
#include "Item/CItem_Crop.h"
#include "CGameInstance.h"
#include "Item/CItem_Crop.h"
#include "Particles/ParticleEmitter.h"

ACBase_Crop::ACBase_Crop()
	: OwnerField(nullptr)
{
	// Mesh
	CHelpers::CreateSceneComponent(this, &MeshComp, "MeshComp", RootComponent);
	
	// Box Component
	CHelpers::CreateSceneComponent(this, &BoxComp, "BoxComp", RootComponent);

	// Mosisture & Nutrition
	CHelpers::CreateActorComponent(this, &MoistureComp, "MoistureComp");
	CHelpers::CreateActorComponent(this, &NutritionComp, "NutritionComp");
	CHelpers::CreateActorComponent(this, &HealthComp, "HealthComp");

	// Set Box Component
	BoxComp->SetBoxExtent(FVector(64.0f, 64.0f, 96.0f));
	BoxComp->SetRelativeLocation(FVector(0.0f, 0.0f, 96.0f));
	BoxComp->SetCollisionProfileName(TEXT("ActionInteractObject"));

	// Property
	CurrentGrowLevel = 0;
	CurrentGrowValue = 0.0f;
	UpdateTime = 1.0f;

	SetType(EInteractObjectType::Crop);
}

void ACBase_Crop::BeginPlay()
{
	Super::BeginPlay();

	HealthComp->OnHealthChanged.AddDynamic(this, &ACBase_Crop::ChangeQualityByHealth);
	SetInteractable();
	GrowUp();
	SetAutoGrowTimer(UpdateTime, true, UpdateTime);
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
	if (IsHarvestable())
	{
		DoHarvest();
		Destroy();
	}

	SetUnInteractable();
}

bool ACBase_Crop::OnHovered()
{
	UE_LOG(LogTemp, Warning, TEXT("%s is Hovered!"), *GetInteractName().ToString());
	MeshComp->SetRenderCustomDepth(true);
	MeshComp->SetCustomDepthStencilValue(1);

	return false;
}

bool ACBase_Crop::OnUnhovered()
{
	UE_LOG(LogTemp, Warning, TEXT("%s is Unhovered!"), *GetInteractName().ToString());
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
	UGameInstance* Instance = GetGameInstance();
	if (Instance)
	{
		UCGameInstance* MyInstance = Cast<UCGameInstance>(Instance);
		if (MyInstance)
		{
			if (const TOptional<FCropData>& CropDataOpt = MyInstance->GetCropDefaultData(CropName))
			{
				if (!CropDataOpt.IsSet())
				{
					return;
				}
				else
				{
					const FCropData& CropData = CropDataOpt.GetValue();
					if (CurrentGrowLevel == 0)
					{
						CHelpers::GetAssetDynamic(&GrowthParticleEffect, CropData.GrowUpPraticleEffectRef);
						CHelpers::GetAssetDynamic(&GrowthSoundEffect, CropData.GrowUpSoundEffectRef);
					}
					CheckTrue(CurrentGrowLevel == CropData.MaxLevel);

					PlayGrowthEffects();
				}
			}
			++CurrentGrowLevel;
			SetCropDatas();

			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't get GameInstance. %s : "), *CropName.ToString());
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

bool ACBase_Crop::IsHarvestable()
{
	if (IsDead())
	{
		return false;
	}
	UGameInstance* Instance = GetGameInstance();
	if (Instance)
	{
		UCGameInstance* MyInstance = Cast<UCGameInstance>(Instance);
		if (MyInstance)
		{
			const TOptional<FCropData>& CropDataOpt = MyInstance->GetCropDefaultData(CropName);
			if (CropDataOpt.IsSet())
			{
				const FCropData& CropData = CropDataOpt.GetValue();
				return CurrentGrowLevel == CropData.MaxLevel;
			}
		}
	}
	return false;
}

void ACBase_Crop::DoHarvest()
{
	if (IsDead())
	{
		UE_LOG(LogTemp, Error, TEXT("Crop is dead!. CropName : %s"), *CropName.ToString());
		return;
	}
	UWorld* World = GetWorld();
	if (World)
	{
		UGameInstance* GI = GetGameInstance();
		if (UCGameInstance* GameInstance = Cast<UCGameInstance>(GI))
		{
			if (TOptional<FCropData> CropDataOpt = GameInstance->GetCropDefaultData(CropName))
			{
				FCropData Data = CropDataOpt.GetValue();
				TOptional<FItemAssetData> ItemAssetDataOpt;
				switch (GetCropQuality())
				{
				case EQualityType::Low:
				{
					ItemAssetDataOpt = GameInstance->GetItemtAssetData(Data.IDForQuality[0]);
				}
				break;
				case EQualityType::Normal:
				{
					ItemAssetDataOpt = GameInstance->GetItemtAssetData(Data.IDForQuality[1]);
				}
				break;
				case EQualityType::High:
				{
					ItemAssetDataOpt = GameInstance->GetItemtAssetData(Data.IDForQuality[2]);
				}
				break;
				default:
				{
					UE_LOG(LogTemp, Error, TEXT("Creation of crop item failed. Invalid Quality. %s"), *(UEnum::GetValueAsString(GetCropQuality())));
					return;
				}
				break;
				}
				if (ItemAssetDataOpt.IsSet())
				{
					FItemAssetData ItemAssetData = ItemAssetDataOpt.GetValue();
					TSubclassOf<ACItem_Crop> CropItemClass;
					CHelpers::GetClass(&CropItemClass, ItemAssetData.ItemClassRef);
					if (CropItemClass)
					{
						for (auto& SpawnPoint : SpawnPoints)
						{
							ACItem_Crop* CropItem = World->SpawnActorDeferred<ACItem_Crop>(CropItemClass, SpawnPoint);
							if (!CropItem)
							{
								UE_LOG(LogTemp, Error, TEXT("Failed to spawn crop item actor."));
								return;
							}
							CropItem->FinishSpawning(SpawnPoint);
						}
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Creation of crop item failed. Invalid Clas Ref. %s"), *(UEnum::GetValueAsString(ItemAssetData.ItemID)));
						return;
					}
				}
			}
		}
	}
}

void ACBase_Crop::SetCropQuality(EQualityType InType)
{
	CropQuality = InType;
}

void ACBase_Crop::ChangeQualityByHealth(float CureentHealth, float PrevHealth, float MaxHealth)
{
	if (HealthComp->GetCurrentHealth() / HealthComp->GetMaxHealth() < 0.3f)
	{
		SetCropQuality(EQualityType::Low);
	}
	else if (HealthComp->GetCurrentHealth() / HealthComp->GetMaxHealth() < 0.8f)
	{
		SetCropQuality(EQualityType::Normal);
	}
	else
	{
		SetCropQuality(EQualityType::High);
	}
}

void ACBase_Crop::AutoGrow()
{
	UGameInstance* Instance = GetGameInstance();
	if (Instance)
	{
		UCGameInstance* MyInstance = Cast<UCGameInstance>(Instance);
		if (MyInstance)
		{
			const TOptional<FCropGrowthData>& GrowthDataOpt = MyInstance->GetCropGrowthData(CropName, CurrentGrowLevel);
			if (GrowthDataOpt.IsSet())
			{
				const FCropGrowthData& GrowthData = GrowthDataOpt.GetValue();
				if (OwnerField)
				{
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
				}

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
			else
			{
				UE_LOG(LogTemp, Error, TEXT("GowthData doesn't set. %s : "), *CropName.ToString());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't get GameInstance. %s : "), *CropName.ToString());
	}
}

void ACBase_Crop::SetCropDatas()
{
	UGameInstance* Instance = GetGameInstance();
	if (Instance)
	{
		UCGameInstance* MyInstance = Cast<UCGameInstance>(Instance);
		if (MyInstance)
		{
			const TOptional<FCropGrowthData>& GrowthDataOpt = MyInstance->GetCropGrowthData(CropName, CurrentGrowLevel);
			if (GrowthDataOpt.IsSet())
			{
				const FCropGrowthData& CurrentGrowthData = GrowthDataOpt.GetValue();

				UStaticMesh* MeshAsset;

				CHelpers::GetAssetDynamic(&MeshAsset, CurrentGrowthData.MeshRef);
				if (!IsValid(MeshAsset))
				{
					UE_LOG(LogTemp, Error, TEXT("Can't find Mesh Asset at %s"), *CurrentGrowthData.MeshRef);
				}
				else
				{
					MeshComp->SetStaticMesh(MeshAsset);
				}
				//MeshComp->SetStaticMesh(CropMeshes[CurrentGrowLevel]);
				NutritionComp->SetSafeRange(CurrentGrowthData.SafeRange_Nutrition);
				MoistureComp->SetSafeRange(CurrentGrowthData.SafeRange_Moisture);
				HealthComp->SetMaxHealth(CurrentGrowthData.Max_Health, true);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("GowthData doesn't set. %s : "), *CropName.ToString());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't get GameInstance. %s : "), *CropName.ToString());
	}
}

