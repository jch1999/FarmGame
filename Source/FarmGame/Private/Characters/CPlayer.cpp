#include "Characters/CPlayer.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CAttributeComponent.h"
#include "Components/COptionComponent.h"
#include "Components/SphereComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Interfaces/CItemInterface.h"
#include "CHUD.h"
#include "UI/CHUDWidget.h"

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
	TraceRange = 150.0f;
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
	
	SetDetectInterval(0.2f);
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPlayer::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPlayer::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ACPlayer::OnInteract);
		EnhancedInputComponent->BindAction(ScrollAction, ETriggerEvent::Started, this, &ACPlayer::Scroll);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
	}
}

void ACPlayer::AddInteractableObject(AActor* InActor)
{
	if (InActor && InActor->Implements<UCInterface_Interactable>())
	{
		/*TScriptInterface<ICInterface_Interactable> InterfaceObj;
		InterfaceObj.SetObject(InActor);
		InterfaceObj.SetInterface(Cast<ICInterface_Interactable>(InActor));*/
		
		int32 BeforeLen = InteractableObjects.Num();
		InteractableObjects.AddUnique(InActor);

		if (BeforeLen != InteractableObjects.Num())
		{
			APlayerController* PC = Cast<APlayerController>(GetController());
			if (PC)
			{
				ACHUD* MyHud = Cast<ACHUD>(PC->GetHUD());
				if (MyHud)
				{
					UCHUDWidget* HudWidget = MyHud->GetHUD();
					HudWidget->AddInteractRow(InActor);
				}
			}
			//UE_LOG(LogTemp, Warning, TEXT("Added Interactable Object: %s"), *InActor->GetName());
		}
	}
}

void ACPlayer::RemoveInteractableObject(AActor* InActor)
{
	if (InActor && InActor->Implements<UCInterface_Interactable>())
	{
		/*TScriptInterface<ICInterface_Interactable> InterfaceObj;
		InterfaceObj.SetObject(InActor);
		InterfaceObj.SetInterface(Cast<ICInterface_Interactable>(InActor));*/

		int32 BeforeLen = InteractableObjects.Num();
		InteractableObjects.RemoveAll([&](AActor* Actor) {return Actor == InActor; });
		int32 AfterLen = InteractableObjects.Num();;

		if (InteractIndex >= AfterLen)
		{
			InteractIndex = FMath::Max(0, AfterLen - 1);
		}

		if (BeforeLen != AfterLen)
		{
			APlayerController* PC = Cast<APlayerController>(GetController());
			if (PC)
			{
				ACHUD* MyHud = Cast<ACHUD>(PC->GetHUD());
				if (MyHud)
				{
					UCHUDWidget* HudWidget = MyHud->GetHUD();
					HudWidget->RemoveInteractRow(InActor);
					HudWidget->SetInteractIndex(InteractIndex);
				}
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("Removed Interactable Object: %s"), *GetNameSafe(InActor));
	}
}

bool ACPlayer::SetDetectInterval(float InTime)
{
	if (InTime < 0.0f) return false;

	DetectInterval = InTime;
	if (GetWorld()->GetTimerManager().TimerExists(DetectTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(DetectTimer);
	}
	GetWorld()->GetTimerManager().SetTimer(DetectTimer, this, &ACPlayer::DetectInteractableObjects, DetectInterval, true);

	return true;
}

bool ACPlayer::SetTraceRange(float InRange)
{
	if (InRange < 0.0f) return false;

	TraceRange = InRange;
	return true;
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
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastScrollTime < ScrollCooldown)
	{
		return;
	}
	LastScrollTime = CurrentTime;

	float inputValue = Value.Get<float>();

	if (InteractableObjects.Num() > 0)
	{
		int32 PreviousIndex = InteractIndex;
		if (inputValue > 0)
		{
			InteractIndex = (InteractIndex + 1) % InteractableObjects.Num();
		}
		else if (inputValue < 0)
		{
			InteractIndex = (InteractIndex - 1 + InteractableObjects.Num()) % InteractableObjects.Num();
		}
		InteractIndex = FMath::Clamp(InteractIndex, 0, InteractableObjects.Num() - 1);

		UE_LOG(LogTemp, Warning, TEXT("Scroll: PrevIndex: %d -> NewIndex: %d, Now Side: %d"), PreviousIndex, InteractIndex, InteractableObjects.Num());

		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			ACHUD* MyHud = Cast<ACHUD>(PC->GetHUD());
			if (MyHud)
			{
				UCHUDWidget* HudWidget = MyHud->GetHUD();
				if (HudWidget)
				{
					HudWidget->SetInteractIndex(InteractIndex);
				}
			}
		}
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
	ICInterface_Interactable* InteractActor = Cast<ICInterface_Interactable>(InteractableObjects[InteractIndex]);
	InteractActor->Interact(this);
	RemoveInteractableObject(InteractableObjects[InteractIndex]);
}

void ACPlayer::DetectInteractableObjects()
{
	TArray<AActor*> ToRemoveActors;
	for (auto Obj : InteractableObjects)
	{
		if (GetDistanceTo(Obj) > TraceRange)
		{
			ToRemoveActors.Add(Obj);
		}
	}
	
	for (auto RemoveActor : ToRemoveActors)
	{
		RemoveInteractableObject(RemoveActor);
	}
	
	if (InteractIndex >= InteractableObjects.Num())
	{
		InteractIndex = FMath::Max(0, InteractableObjects.Num() - 1);
	}

	Trace(ECollisionChannel::ECC_GameTraceChannel1);

}

void ACPlayer::SetType(EInteractObjectType InNewType)
{
	InteractType = InNewType;
}

bool ACPlayer::Trace(ECollisionChannel TraceChannel)
{
	//UE_LOG(LogTemp, Warning, TEXT("TraceChannel: %d"), TraceChannel);

	FVector Start = GetActorLocation() + CameraComp->GetRelativeLocation();
	FVector End = Start + CameraComp->GetForwardVector() * 150.0f;

	//TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> Ignores;
	Ignores.Add(this);

	TArray<FHitResult> Hits;
	UKismetSystemLibrary::SphereTraceMulti
	(
		GetWorld(),
		Start,
		End,
		TraceRange,
		UEngineTypes::ConvertToTraceType(TraceChannel),
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
			if (Hit.GetActor())
			{
				ICInterface_Interactable* OtherActor = Cast<ICInterface_Interactable>(Hit.GetActor());
				if (OtherActor && OtherActor->IsInteractable())
				{
					AddInteractableObject(Hit.GetActor());
				}
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}
