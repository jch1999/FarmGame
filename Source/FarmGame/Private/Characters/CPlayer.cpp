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
#include "Interfaces/CItemInterface.h"
#include "CHUD.h"
#include "UI/CHUDWidget.h"
#include "UI/CQuickSlotBarWidget.h"
#include "UI/CQuickSlotWidget.h"
#include "Item/CItemBase.h"
#include "CGameInstance.h"

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

	// Animation
	TSubclassOf<UAnimInstance> AnimClass;
	CHelpers::GetClass(&AnimClass, "/Game/Player/ABP_CPlayer");
	GetMesh()->SetAnimInstanceClass(AnimClass);

	// State Comp
	CHelpers::CreateActorComponent(this, &StateComp, "StateComp");

	// Attribute Comp
	CHelpers::CreateActorComponent(this, &AttributeComp, "AttributeComp");

	// Option Comp
	CHelpers::CreateActorComponent(this, &OptionComp, "OptionComp");

	// Interact Comp
	CHelpers::CreateActorComponent(this, &InteractComp, "InteractComp");
	
	// Inventory Comp
	CHelpers::CreateActorComponent(this, &InventoryComp, "InventoryComp");
	
	// Movement Comp
	GetCharacterMovement()->MaxWalkSpeed = AttributeComp->GetWalkSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	SetInteractable();
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

void ACPlayer::StartPlantingAnimation()
{
	if (PlantAnim)
	{
		PlayAnimMontage(PlantAnim);

		float MontageDuration = PlantAnim->GetPlayLength();
		FTimerHandle PlantingAnimTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(PlantingAnimTimerHandle, this, &ACPlayer::OnPlantingAnimationFinished, MontageDuration, false);
	}
}

void ACPlayer::OnPlantingAnimationFinished()
{
	// if(InteractComp->)
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
	if (CurrentEquippedItem)
	{
		CurrentEquippedItem->Destroy();
		CurrentEquippedItem = nullptr;
	}

	int32 TargetSlotIndex = QuickSlotBar->GetCurrentSlot()->TargetSlotIndex;
	if (TargetSlotIndex < 0) return;

	const FInventorySlot* const SlotData = &(InventoryComp->GetSlotDatas()[TargetSlotIndex]);
	if (!SlotData || SlotData->ItemID == EItemID::None) return;
	

	// 새 아이템 생성
	FTransform SpawnTransform;
	CurrentEquippedItem = GetWorld()->SpawnActor<ACItemBase>(SlotData->ItemClass, SpawnTransform);

	if (CurrentEquippedItem)
	{
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
					}
				}
			}
		}
	}
}


bool ACPlayer::OnHovered()
{
	return false;
}

bool ACPlayer::OnUnhovered()
{
	return false;
}