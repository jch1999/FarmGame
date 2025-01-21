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
#include "Interfaces/CItemInterface.h"

ACPlayer::ACPlayer()
{
	// Input Setting
	CHelpers::GetAsset(&DefaultContext, "/Game/Input/IMC_Player");
	CHelpers::GetAsset(&MoveAction, "/Game/Input/IA_PlayerMove");
	CHelpers::GetAsset(&LookAction, "/Game/Input/IA_PlayerRotate");
	CHelpers::GetAsset(&InteractAction, "/Game/Input/IA_PlayerInteract");
	CHelpers::GetAsset(&ScrollAction, "/Game/Input/IA_PlayerScroll");

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

	InteractIndex = 0;
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

	ItemContainer.SetNum(ItemContainerSize);
	SetInteractable();
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
		EnhancedInputComponent->BindAction(ScrollAction, ETriggerEvent::Triggered, this, &ACPlayer::Scroll);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
	}
}

void ACPlayer::AddInteractableObject(UObject* InObj)
{
	if (InObj && InObj->Implements<UCInterface_Interactable>())
	{
		TScriptInterface<ICInterface_Interactable> InterfaceObj;
		InterfaceObj.SetObject(InObj);
		InterfaceObj.SetInterface(Cast<ICInterface_Interactable>(InObj));

		InteractableObjects.Add(InterfaceObj);
	}
}

void ACPlayer::RemoveInteractableObject(UObject* InObj)
{
	if (InObj && InObj->Implements<UCInterface_Interactable>())
	{
		TScriptInterface<ICInterface_Interactable> InterfaceObj;
		InterfaceObj.SetObject(InObj);
		InterfaceObj.SetInterface(Cast<ICInterface_Interactable>(InObj));

		InteractableObjects.Remove(InterfaceObj);
	}
}

void ACPlayer::Move(const FInputActionValue& Value)
{
	FVector2D Axis2D = Value.Get<FVector2D>();
	
	if (!Controller) return;
	
	FRotator ControlRot = FRotator(0, Controller->GetControlRotation().Yaw, 0);
	FVector ForwardDirection = FQuat(ControlRot).GetForwardVector();
	FVector SideDirection = FQuat(ControlRot).GetRightVector();

	AddMovementInput(ForwardDirection, Axis2D.Y);
	AddMovementInput(SideDirection, Axis2D.X);
}

void ACPlayer::Look(const FInputActionValue& Value)
{
	if (!Controller)return;

	FVector2D InputValue = Value.Get<FVector2D>();
	InputValue.X *= OptionComp->GetMouseXSpeed() * GetWorld()->GetDeltaSeconds();
	InputValue.Y *= OptionComp->GetMouseYSpeed() * GetWorld()->GetDeltaSeconds();
	AddControllerYawInput(InputValue.X);
	AddControllerPitchInput(-InputValue.Y);
}

void ACPlayer::OnInteract(const FInputActionInstance& InInstance)
{
	bool bValue = InInstance.GetValue().Get<bool>();

	if (bValue)
	{
		Interact(nullptr);
	}
}

void ACPlayer::Scroll(const FInputActionValue& Value)
{
	float inputValue = Value.Get<float>();

	if (InteractableObjects.Num() > 0)
	{
		if (inputValue > 0)
		{
			++InteractIndex;
		}
		else if (inputValue < 0)
		{
			--InteractIndex;
		}
		InteractIndex %= InteractableObjects.Num();
	}
	else
	{
		InteractIndex = 0;
	}
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

void ACPlayer::Interact(AActor* OtherActor)
{
	if (InteractableObjects.Num() == 0) return;

	/*ICInterface_Interactable* InteractActor=nullptr;
	if (Trace(InteractActor))
	{
		APlayerController* PC = GetController<APlayerController>();
		CheckNull(PC);

		PC->bShowMouseCursor = true;
		PC->SetInputMode(FInputModeGameAndUI());
	}*/
	InteractableObjects[InteractIndex]->Interact(this);
}

void ACPlayer::SetType(EInteractObjectType InNewType)
{
	InteractType = InNewType;
}

bool ACPlayer::Trace(const ICInterface_Interactable* OutInteract)
{
	FVector Start = GetActorLocation() + CameraComp->GetRelativeLocation();
	FVector End = Start + CameraComp->GetForwardVector() * 150.0f;

	//TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> Ignores;
	Ignores.Add(this);

	TArray<FHitResult> Hits;
	UKismetSystemLibrary::LineTraceMulti
	(
		GetWorld(),
		Start,
		End,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
		false,
		Ignores,
		EDrawDebugTrace::ForDuration,
		Hits,
		true
	);

	if (Hits.Num() > 0)
	{
		for (FHitResult& Hit : Hits)
		{
			ICInterface_Interactable* OtherActor = Cast<ICInterface_Interactable>(Hit.GetActor());
			if (OtherActor && OtherActor->IsInteractable())
			{
				OtherActor->Interact(this);
				OutInteract = OtherActor;
				return true;
			}
		}
	}
	return false;
}
