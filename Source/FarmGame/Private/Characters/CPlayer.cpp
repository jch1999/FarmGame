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
	ItemContainer.SetNum(ItemContainerSize);

	OriginalCameraRelativeLocation = GetActorRotation().UnrotateVector(CameraComp->GetComponentLocation() - GetActorLocation());
	OriginalCameraRelativeRotation = CameraComp->GetComponentRotation() - GetActorRotation();
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

void ACPlayer::MoveCameraToLocation(const FVector& TargetLocation, const FRotator& TargetRotation)
{
	CameraStartLocation = CameraComp->GetComponentLocation();
	CameraStartRotation = CameraComp->GetComponentRotation();
	CameraTargetLocation = TargetLocation;
	CameraTargetRotation = TargetRotation;
	CameraLerpAlpha = 0.0f;

	GetWorld()->GetTimerManager().SetTimer(CameraLerpTimerHandle, this, &ACPlayer::UpdateCameraLerp, 0.01f, true);
}

void ACPlayer::RestoreCamera()
{
	CameraStartLocation = CameraComp->GetComponentLocation();
	CameraStartRotation = CameraComp->GetComponentRotation();
	CameraTargetLocation = GetActorLocation() + GetActorRotation().RotateVector(OriginalCameraRelativeLocation);
	CameraTargetRotation = GetActorRotation() + OriginalCameraRelativeRotation;
	CameraLerpAlpha = 0.0f;

	GetWorld()->GetTimerManager().SetTimer(CameraLerpTimerHandle, this, &ACPlayer::UpdateCameraLerp, 0.01f, true);
}

void ACPlayer::UpdateCameraLerp()
{
	CameraLerpAlpha += 0.05f;
	if (CameraLerpAlpha >= 1.0f)
	{
		CameraLerpAlpha = 1.0f;
		GetWorld()->GetTimerManager().ClearTimer(CameraLerpTimerHandle);
	}

	FVector NewLocation = FMath::Lerp(CameraStartLocation, CameraTargetLocation, CameraLerpAlpha);
	FRotator NewRotation = FMath::Lerp(CameraStartRotation, CameraTargetRotation, CameraLerpAlpha);

	CameraComp->SetWorldLocationAndRotation(NewLocation, NewRotation);
}

bool ACPlayer::OnHovered()
{
	return false;
}

bool ACPlayer::OnUnhovered()
{
	return false;
}