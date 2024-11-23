#include "Characters/CPlayer.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CAttributeComponent.h"
#include "Components/COptionComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

ACPlayer::ACPlayer()
{
	// Input Setting
	CHelpers::GetAsset(&DefaultContext, "/Game/Input/IMC_Player");
	CHelpers::GetAsset(&MoveAction, "/Game/Input/IA_PlayerMove");
	CHelpers::GetAsset(&LookAction, "/Game/Input/IA_PlayerRotate");
	CHelpers::GetAsset(&LookAction, "/Game/Input/IA_PlayerInteract");

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
	CHelpers::GetAsset(&MeshAsset, "/Game/3D_LOW_POLY_FarmerPack/Characters/Mesh/SKM_Farmer_male");
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

	// Movement Comp
	GetCharacterMovement()->MaxWalkSpeed = AttributeComp->GetWalkSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	// Interactable
	SetType(EInteractObjectType::Player);
	SetInteractable();
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// Set MappingContext to LocalPalyer's SubSystem
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (SubSystem)
		{
			SubSystem->AddMappingContext(DefaultContext, 0);
		}
	}
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPlayer::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPlayer::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACPlayer::OnInteract);
	}
}

void ACPlayer::Move(const FInputActionInstance& InInstance)
{
	FVector2D Axis2D = InInstance.GetValue().Get<FVector2D>();
	
	FRotator ControlRot = FRotator(0, GetControlRotation().Yaw, 0);
	FVector ForwardDirection = FQuat(ControlRot).GetForwardVector();
	FVector SideDirection = FQuat(ControlRot).GetRightVector();

	PrintLine();
	AddMovementInput(ForwardDirection, Axis2D.Y);
	AddMovementInput(SideDirection, Axis2D.X);
}

void ACPlayer::Look(const FInputActionInstance& InInstance)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		FVector2D InputValue = InInstance.GetValue().Get<FVector2D>();
		InputValue.X *= OptionComp->GetMouseXSpeed() * GetWorld()->GetDeltaSeconds();
		InputValue.Y *= OptionComp->GetMouseYSpeed() * GetWorld()->GetDeltaSeconds();
		AddControllerYawInput(InputValue.X);
		AddControllerPitchInput(-InputValue.Y);
	}
}

void ACPlayer::OnInteract(const FInputActionInstance& InInstance)
{
	bool bValue = InInstance.GetValue().Get<bool>();

	if (bValue)
	{
		Interact();
	}
}

// override from ICInterface_Interactable
bool ACPlayer::IsInteractable()
{
	return bInteractable;
}

void ACPlayer::SetInteractable()
{
	bInteractable = true;
}

void ACPlayer::SetUnInteractable()
{
	bInteractable = false;
}

void ACPlayer::Interact()
{
	CLog::Log("Interact Test!");
}

EInteractObjectType ACPlayer::GetType()
{
	return Type;
}

void ACPlayer::SetType(EInteractObjectType InNewType)
{
	Type = InNewType;
}

