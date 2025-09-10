#include "Characters/CPlayer.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CAttributeComponent.h"
#include "Components/COptionComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CInteractComponent.h"
#include "Components/CInventoryComponent.h"
#include "Components/PostProcessComponent.h"
#include "Interfaces/CItemInterface.h"
#include "CHUD.h"
#include "UI/CHUDWidget.h"
#include "UI/CQuickSlotBarWidget.h"
#include "UI/CQuickSlotWidget.h"
#include "Item/CItemBase.h"
#include "Item/CItem_Seed.h"
#include "CGameInstance.h"
#include "Farm/CFarmField.h"
#include "Controller/CPlayerController.h"

ACPlayer::ACPlayer()
{
	// SpringArm Comp
	CHelpers::CreateSceneComponent(this, &SpringArmComp, "SpringArmComp", GetMesh());
	SpringArmComp->SetRelativeLocation(FVector(0, 0, 140));
	SpringArmComp->AddRelativeRotation(FRotator(0, 90, 0));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraLag = true;

	// Camera Comp
	CHelpers::CreateSceneComponent(this, &CameraComp, "CameraComp", SpringArmComp);

	//MeshComp
	USkeletalMesh* MeshAsset;
	CHelpers::GetAsset(&MeshAsset, "/Game/ThirdParty/3D_LOW_POLY_FarmerPack/Characters/Mesh/SKM_Farmer_male");
	GetMesh()->SetSkeletalMesh(MeshAsset);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(10);
	// Animation
	TSubclassOf<UAnimInstance> AnimClass;
	CHelpers::GetClass(&AnimClass, "/Game/Player/ABP_CPlayer");
	GetMesh()->SetAnimInstanceClass(AnimClass);

	// State Comp
	//CHelpers::CreateActorComponent(this, &StateComp, "StateComp");

	// Attribute Comp
	CHelpers::CreateActorComponent(this, &AttributeComp, "AttributeComp");

	// Option Comp
	//CHelpers::CreateActorComponent(this, &OptionComp, "OptionComp");

	// Interact Comp
	CHelpers::CreateActorComponent(this, &InteractComp, "InteractComp");
	
	// Inventory Comp
	CHelpers::CreateActorComponent(this, &InventoryComp, "InventoryComp");
	
	// Movement Comp
	GetCharacterMovement()->MaxWalkSpeed = AttributeComp->GetWalkSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	// Post Process Comp
	/*CHelpers::CreateSceneComponent(this, &PostProcessComp, "PostProcessComp", GetMesh());
	PostProcessComp->bUnbound = true;
	PostProcessComp->BlendWeight = 1.0f;*/

	// For LookAt & Align
	TargetActor = nullptr;
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	SetInteractable();

	// Fade
	//UMaterialInterface* Mat;
	//CHelpers::GetAssetDynamic(&Mat, "/Game/ThirdParty/3D_LOW_POLY_FarmerPack/Material/M_farm1_Inst");
	//if (Mat)
	//{
	//	FadeMaterialInstance = UMaterialInstanceDynamic::Create(Mat, this);
	//	if (FadeMaterialInstance)
	//	{
	//		GetMesh()->SetMaterial(0, FadeMaterialInstance);
	//		//PostProcessComp->Settings.WeightedBlendables.Array.Add(FWeightedBlendable(1.0f, FadeMaterialInstance));
	//	}
	//}
	//TargetOpacity = 1.0f;
	//CurrentOpacity = 1.0f;
	//FadeLerpDuration = 0.5f;
	//FadeLerpElapsed = 0.0f;
	//bFadingOut = false;
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// override from ICInterface_Interactable

void ACPlayer::SetInteractable()
{
	bInteractable = true;
}

void ACPlayer::SetUnInteractable()
{
	bInteractable = false;
}

void ACPlayer::SetType(EInteractObjectType InNewType)
{
	InteractType = InNewType;
}

void ACPlayer::Interact(AActor* OtherActor)
{
	if (!InteractComp) return;
	
	InteractComp->DoInteract(nullptr);
}

void ACPlayer::ActionInteract()
{
	if (!InteractComp) return;

	InteractComp->DoActionInteract();
}

ACItemBase* ACPlayer::GetCurretnEquippedItem()
{
	return CurrentEquippedItem;
}

void ACPlayer::ResetEquippedItem()
{
	if (IsValid(CurrentEquippedItem))
	{
		InventoryComp->OnInventorySlotDataUpdated.RemoveDynamic(CurrentEquippedItem, &ACItemBase::UpdateByInventory_DataUpdated);
		InventoryComp->OnInventorySlotSwap.RemoveDynamic(CurrentEquippedItem, &ACItemBase::UpdateByInventory_SlotSwap);
		CurrentEquippedItem->Destroy();
	}
	CurrentEquippedItem = nullptr;
}

void ACPlayer::StartPlantingAnimation()
{
	if (PlantAnim)
	{
		if (ACPlayerController* MyController = Cast<ACPlayerController>(GetController()))
		{
			// 중복 재생 방지
			if (!(MyController->GetStateComponent()->IsIdleMode()))
			{
				return;
			}
			MyController->SetUnSlotChangable();
			MyController->GetStateComponent()->SetActionMode();
		}
		if (IsValid(InteractComp->GetActionInteractTarget()))
		{
			if (ACFarmField* TargetFramField = Cast<ACFarmField>(InteractComp->GetActionInteractTarget()))
			{
				if (PendingActionInteract)
				{
					UE_LOG(LogTemp, Warning, TEXT("PendingActionInteract already set, overwriting!"));
				}

				PendingActionInteract = [this]()
				{
					PlayAnimMontage(PlantAnim);
				};
				TargetActor = TargetFramField;
				AlignToActor(TargetFramField, TargetFramField->GetPlantOffset(), true);
				// LookAtActor(InteractComp->GetActionInteractTarget(), true);
				
				UE_LOG(LogTemp, Warning, TEXT("Play Plant Anim"));
				/*float MontageDuration = PlantAnim->GetPlayLength();
				FTimerHandle PlantingAnimTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(PlantingAnimTimerHandle, this, &ACPlayer::OnPlantingAnimationFinished, MontageDuration, false);*/
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Plant Animation is missing!"));
	}
}

void ACPlayer::OnPlantingAnimationFinished()
{
	if (ACFarmField* FarmField = Cast<ACFarmField>(InteractComp->GetActionInteractTarget()))
	{
		if (ACItem_Seed* Seed = Cast<ACItem_Seed>(CurrentEquippedItem))
		{
			FarmField->PlantCrop(Seed->CropClass, Seed->PlantLocation);
			Seed->UseItem();
			UE_LOG(LogTemp, Warning, TEXT("Plant Crop"));
		}
	}


	if (ACPlayerController* MyController = Cast<ACPlayerController>(GetController()))
	{
		MyController->SetSlotChangable();
		MyController->GetStateComponent()->SetIdleMode();
	}

	if (ACFarmField* Target = Cast<ACFarmField>(InteractComp->GetActionInteractTarget()))
	{
		Target->SetInteractable();
		if (!Target->IsInteractable()) return;
		Target->OnHovered();
	}
}

void ACPlayer::StartWateringAnimation()
{
	if (WateringAnim)
	{
		if (ACPlayerController* MyController = Cast<ACPlayerController>(GetController()))
		{// 중복 재생 방지
			if (!(MyController->GetStateComponent()->IsIdleMode()))
			{
				return;
			}
			MyController->SetUnSlotChangable();
			MyController->GetStateComponent()->SetActionMode();
		}
		if (IsValid(InteractComp->GetActionInteractTarget()))
		{
			if (ACFarmField* TargetFramField = Cast<ACFarmField>(InteractComp->GetActionInteractTarget()))
			{
				if (PendingActionInteract)
				{
					UE_LOG(LogTemp, Warning, TEXT("PendingActionInteract already set, overwriting!"));
				}

				PendingActionInteract = [this]()
				{
					PlayAnimMontage(WateringAnim);
				};
				TargetActor = TargetFramField;
				AlignToActor(TargetFramField, TargetFramField->GetWaterOffset(), true);
				//LookAtActor(InteractComp->GetActionInteractTarget(), true);
				UE_LOG(LogTemp, Warning, TEXT("Play Plant Anim"));
			}
		}
	}
}

void ACPlayer::OnWateringAnimationFinished()
{
	if (ACPlayerController* MyController = Cast<ACPlayerController>(GetController()))
	{
		MyController->SetSlotChangable();
		MyController->GetStateComponent()->SetIdleMode();
	}

	if (ICInterface_Interactable* Target = Cast<ICInterface_Interactable>(InteractComp->GetActionInteractTarget()))
	{
		if (!Target->IsInteractable()) return;
		Target->OnHovered();
	}
}

UCQuickSlotBarWidget* ACPlayer::GetQuickSlotBar()
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ACHUD* MyHUD = Cast<ACHUD>(PC->GetHUD()))
		{
			return MyHUD->GetHUD()->GetQuickSlotBar();
		}
	}
	return nullptr;
}

void ACPlayer::EquipItemFromQuickSlot(int32 QuickSlotIndex)
{
	UCQuickSlotBarWidget* QuickSlotBar = GetQuickSlotBar();
	if (!QuickSlotBar) return;

	if (!InventoryComp) return;
	
	// 기존 아이템 제거
	ResetEquippedItem();

	int32 TargetSlotIndex = QuickSlotBar->GetCurrentSlot()->TargetSlotIndex;
	UE_LOG(LogTemp, Warning, TEXT("TargetSlotIndex :%d"), TargetSlotIndex);
	if (TargetSlotIndex < 0) return;

	const FInventorySlot* const SlotData = &(InventoryComp->GetSlotDatas()[TargetSlotIndex]);
	if ((!SlotData) || SlotData->ItemID == EItemID::None) return;
	

	// 새 아이템 생성
	FTransform SpawnTransform;
	CurrentEquippedItem = GetWorld()->SpawnActor<ACItemBase>(SlotData->ItemClass, SpawnTransform);
	// Set QuickSlotIndex for CurrentEquippedItem
	

	if (CurrentEquippedItem)
	{
		CurrentEquippedItem->SetOwner(this);
		CurrentEquippedItem->SetTargetSlotIndex(TargetSlotIndex);
		CurrentEquippedItem->SetOwnerCharacter(this);
		CurrentEquippedItem->SetUsable();
		InventoryComp->OnInventorySlotDataUpdated.AddDynamic(CurrentEquippedItem, &ACItemBase::UpdateByInventory_DataUpdated);
		InventoryComp->OnInventorySlotSwap.AddDynamic(CurrentEquippedItem, &ACItemBase::UpdateByInventory_SlotSwap);
		TArray<int32> Indexes;
		Indexes.Add(CurrentEquippedItem->GetTargetSlotIndex());
		CurrentEquippedItem->UpdateByInventory_DataUpdated(Indexes);

		if (UGameInstance* GI = GetGameInstance())
		{
			if (UCGameInstance* MyGI = Cast<UCGameInstance>(GI))
			{
				if (TOptional<FItemAttachData> AttachOpt = MyGI->GetItemAttachData(SlotData->ItemID))
				{
					if (AttachOpt.IsSet())
					{
						FItemAttachData& AttachData = AttachOpt.GetValue();
						FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
						CurrentEquippedItem->AttachToComponent(GetMesh(), AttachRules, AttachData.SocketName);
						CurrentEquippedItem->SetActorRelativeLocation(AttachData.Location);
						CurrentEquippedItem->SetActorRelativeRotation(AttachData.Rotation);
						CurrentEquippedItem->SetActorRelativeScale3D(AttachData.Scale);
						CurrentEquippedItem->SetCollision(false);
						UE_LOG(LogTemp, Warning, TEXT("AttachData.SocketName: %s"), *AttachData.SocketName.ToString());
						UE_LOG(LogTemp, Warning, TEXT("AttachData.Location: %s"), *AttachData.Location.ToString());
						UE_LOG(LogTemp, Warning, TEXT("AttachData.Rotation: %s"), *AttachData.Rotation.ToString());
					}
				}
			}
		}
	}
}

void ACPlayer::SetVisibility(bool bVisible)
{
	GetMesh()->SetVisibility(bVisible);
	if (IsValid(CurrentEquippedItem))
	{
		CurrentEquippedItem->GetItemMesh()->SetVisibility(bVisible);
	}
}

void ACPlayer::LookAtActor(AActor* InActor, bool bInterp)
{
	if (!IsValid(InActor)) return;

	// Looking at action interact target
	FVector PlayerLocation = GetActorLocation();
	FVector TargetActorLocation = InActor->GetActorLocation();
	FVector DirectionVector = TargetActorLocation - PlayerLocation;
	DirectionVector.Z = 0.0f;

	TargetRotation = UKismetMathLibrary::MakeRotFromX(DirectionVector);
	if (bInterp)
	{
		FRotator CurrentRotation = GetActorRotation();
		GetWorld()->GetTimerManager().SetTimer(RotationTimer, this, &ACPlayer::RotationInterp, 0.01f, true);
	}
	else
	{
		SetActorRotation(TargetRotation);
		OnLookAtComplete();
	}
}

void ACPlayer::AlignToActor(AActor* InTargetActor, const FVector& Offset, bool bInterp)
{
	if (!IsValid(InTargetActor)) return;

	TargetActor = InTargetActor;
	TargetLocation = TargetActor->GetActorLocation() + Offset;

	if (bInterp)
	{
		GetWorld()->GetTimerManager().SetTimer(
			AlignTimer, this, &ACPlayer::AlignInterp, 0.01f, true
		);
	}
	else
	{
		SetActorLocation(TargetLocation);
		OnAlignComplete();
	}
}

void ACPlayer::RotationInterp()
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, InterpSpeed);
	SetActorRotation(InterpRotation);

	float YawDiff = FMath::Abs(FMath::FindDeltaAngleDegrees(InterpRotation.Yaw, TargetRotation.Yaw));
	
	if (YawDiff <= 1.0f)
	{
		SetActorRotation(TargetRotation);
		GetWorld()->GetTimerManager().ClearTimer(RotationTimer);

		OnLookAtComplete();
	}
}

void ACPlayer::OnLookAtComplete()
{
	// 몽타주 재생.
	if (PendingActionInteract)
	{
		PendingActionInteract();
		PendingActionInteract = nullptr;
	}
}

void ACPlayer::AlignInterp()
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = FMath::VInterpConstantTo(
		CurrentLocation,
		TargetLocation,
		DeltaTime,
		AlignInterpSpeed
	);

	SetActorLocation(NewLocation);

	float Distance = FVector::Dist(NewLocation, TargetLocation);
	if (Distance < AcceptableDistance)
	{
		SetActorLocation(TargetLocation);
		GetWorld()->GetTimerManager().ClearTimer(AlignTimer);

		OnAlignComplete();
	}
}

void ACPlayer::OnAlignComplete()
{
	if (IsValid(TargetActor))
	{
		LookAtActor(TargetActor, true);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Call LookAtActor Failed! TargetActor is nullptr."));
	}
}

/*void ACPlayer::StartFade(bool bToTransparent)
{
	bFadingOut = bToTransparent;
	TargetOpacity = bToTransparent ? 0.3f : 1.0f;
	FadeLerpElapsed = 0.0f;

	UE_LOG(LogTemp, Warning, TEXT("Target Opacity : %f"), TargetOpacity);
	GetWorld()->GetTimerManager().SetTimer(OpacityTimerHandle, this, &ACPlayer::UpdateFade, 0.01f, true);
}

void ACPlayer::UpdateFade()
{
	FadeLerpElapsed += 0.01f;
	float Alpha = FMath::Clamp(FadeLerpElapsed / FadeLerpDuration, 0.0f, 1.0f);
	float NewOpacity = FMath::Lerp(CurrentOpacity, TargetOpacity, Alpha);

	if (FadeMaterialInstance)
	{
		FadeMaterialInstance->SetScalarParameterValue("FadeAlpha", NewOpacity);
		float Value = 0.0f;
		FadeMaterialInstance->GetScalarParameterValue(FName("FadeAlpha"), Value);
		UE_LOG(LogTemp, Warning, TEXT("Now FadeAlpha : %f"), Value);
	}

	if (Alpha >= 1.0f)
	{
		CurrentOpacity = TargetOpacity;
		GetWorld()->GetTimerManager().ClearTimer(OpacityTimerHandle);
	}
}*/

void ACPlayer::SetDelayedInteractable(float DelayTime)
{
	if (GetWorld()->GetTimerManager().TimerExists(InteractTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractTimer);
	}
	GetWorld()->GetTimerManager().SetTimer(InteractTimer, this, &ACPlayer::SetInteractable, DelayTime, false);
}

void ACPlayer::SetDelayedUninteractable(float DelayTime)
{
	if (GetWorld()->GetTimerManager().TimerExists(InteractTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractTimer);
	}
	GetWorld()->GetTimerManager().SetTimer(InteractTimer, this, &ACPlayer::SetUnInteractable, DelayTime, false);
}

bool ACPlayer::OnHovered()
{
	return false;
}

bool ACPlayer::OnUnhovered()
{
	return false;
}