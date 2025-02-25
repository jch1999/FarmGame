#include "Controller/CPlayerController.h"
#include "CHUD.h"
#include "Global.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Characters/CPlayer.h"
#include "Components/COptionComponent.h"
#include "Components/CInteractComponent.h"
#include "Components/CInventoryComponent.h"
#include "UI/CHUDWidget.h"

ACPlayerController::ACPlayerController()
{
	// Input Setting
	CHelpers::GetAsset(&DefaultContext, "/Game/Input/IMC_Player");
	CHelpers::GetAsset(&MoveAction, "/Game/Input/IA_PlayerMove");
	CHelpers::GetAsset(&LookAction, "/Game/Input/IA_PlayerRotate");
	CHelpers::GetAsset(&InteractAction, "/Game/Input/IA_PlayerInteract");
	CHelpers::GetAsset(&InteractAction, "/Game/Input/IA_PlayerActionInteract");
	CHelpers::GetAsset(&ScrollAction, "/Game/Input/IA_PlayerScroll");
	CHelpers::GetAsset(&OpenInventoryAction, "/Game/Input/IA_OpenInventory");

	CHelpers::GetAsset(&UIContext, "/Game/Input/IMC_UI");
	CHelpers::GetAsset(&CloseInventoryAction, "/Game/Input/IA_CloseInventory");
}

void ACPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if (!IsLocalController())
	{
		UE_LOG(LogTemp, Warning, TEXT("Is not local controller"));
		return;
	}

	AHUD* Hud = GetHUD();
	if (IsValid(Hud))
	{
		ACHUD* MyHud = Cast<ACHUD>(Hud);
		if (IsValid(MyHud))
		{
			MyHud->CreateHUD();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HUD is not valid in OnPossess"));
		}
	}
}

void ACPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	if (!IsLocalController())
	{
		UE_LOG(LogTemp, Warning, TEXT("Is not local controller"));
		return;
	}

	AHUD* Hud = GetHUD();
	if (IsValid(Hud))
	{
		ACHUD* MyHud = Cast<ACHUD>(Hud);
		if (IsValid(MyHud))
		{
			MyHud->CreateHUD();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HUD is not valid in OnPossess"));
		}
	}
}

void ACPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (SubSystem)
	{
		SubSystem->AddMappingContext(DefaultContext, 0);
	}
}

void ACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPlayerController::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPlayerController::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ACPlayerController::OnInteract);
		EnhancedInputComponent->BindAction(ActionInteractAction, ETriggerEvent::Started, this, &ACPlayerController::OnActionInteract);
		EnhancedInputComponent->BindAction(ScrollAction, ETriggerEvent::Started, this, &ACPlayerController::Scroll);
		EnhancedInputComponent->BindAction(OpenInventoryAction, ETriggerEvent::Started, this, &ACPlayerController::OpenInventory);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
	}
}

void ACPlayerController::Move(const FInputActionValue& Value)
{
	ACPlayer* MyPlayer = Cast<ACPlayer>(GetPawn());
	if (!MyPlayer) return;

	FVector2D Axis2D = Value.Get<FVector2D>();

	FRotator ControlRot = FRotator(0, GetControlRotation().Yaw, 0);
	FVector ForwardDirection = FQuat(ControlRot).GetForwardVector();
	FVector SideDirection = FQuat(ControlRot).GetRightVector();

	MyPlayer->AddMovementInput(ForwardDirection, Axis2D.Y);
	MyPlayer->AddMovementInput(SideDirection, Axis2D.X);
}

void ACPlayerController::Look(const FInputActionValue& Value)
{
	ACPlayer* MyPlayer = Cast<ACPlayer>(GetPawn());
	if (!MyPlayer) return;

	FVector2D InputValue = Value.Get<FVector2D>();
	InputValue.X *= MyPlayer->GetOptionComponent()->GetMouseXSpeed() * GetWorld()->GetDeltaSeconds();
	InputValue.Y *= MyPlayer->GetOptionComponent()->GetMouseYSpeed() * GetWorld()->GetDeltaSeconds();

	AddYawInput(InputValue.X);
	AddPitchInput(-InputValue.Y);
}

void ACPlayerController::OnInteract(const FInputActionValue& Value)
{
	ACPlayer* MyPlayer = Cast<ACPlayer>(GetPawn());
	if (MyPlayer)
	{
		MyPlayer->Interact();
	}
}

void ACPlayerController::OnActionInteract(const FInputActionValue& Value)
{
	ACPlayer* MyPlayer = Cast<ACPlayer>(GetPawn());
	if (MyPlayer)
	{
		MyPlayer->ActionInteract();
	}
}

void ACPlayerController::Scroll(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	if (InputValue == 0.0f) return;

	ACPlayer* MyPlayer = Cast<ACPlayer>(GetPawn());
	if (MyPlayer)
	{
		MyPlayer->GetInteractComponent()->Scroll(InputValue);
	}
}

void ACPlayerController::OpenInventory(const FInputActionValue& Value)
{
	ACPlayer* MyPlayer = Cast<ACPlayer>(GetPawn());
	if (MyPlayer)
	{
		SetUIInputMode();
		MyPlayer->GetInventoryComponent()->ShowInventory();
	}
	UE_LOG(LogTemp, Error, TEXT("Can't find Player. Function : ACPlayerController::OpenInventory"));
}

void ACPlayerController::CloseInventory(const FInputActionValue& Value)
{
	ACPlayer* MyPlayer = Cast<ACPlayer>(GetPawn());
	if (MyPlayer)
	{
		MyPlayer->GetInventoryComponent()->HideInventory(); 
		SetGameInputMode();
	}
}

void ACPlayerController::SetUIInputMode()
{
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
	bShowMouseCursor = true;

	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (SubSystem)
	{
		SubSystem->RemoveMappingContext(DefaultContext);
		SubSystem->AddMappingContext(UIContext, 1);
	}

	// UI Action Binding (Once)
	static bool bIsUIActionBound = false;
	if (!bIsUIActionBound)
	{
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
		if (EnhancedInputComponent)
		{
			EnhancedInputComponent->BindAction(CloseInventoryAction, ETriggerEvent::Started, this, &ACPlayerController::SetGameInputMode);
			bIsUIActionBound = true;
		}
	}

	// Hide Hud
	AHUD* Hud = GetHUD();
	if (IsValid(Hud))
	{
		ACHUD* MyHud = Cast<ACHUD>(Hud);
		if (IsValid(MyHud))
		{
			MyHud->GetHUD()->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	UE_LOG(LogTemp, Display, TEXT("Change to UI Input Mode"));
}

void ACPlayerController::SetGameInputMode()
{
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;

	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (SubSystem)
	{
		SubSystem->RemoveMappingContext(UIContext);
		SubSystem->AddMappingContext(DefaultContext, 0);
	}

	// Show Hud
	AHUD* Hud = GetHUD();
	if (IsValid(Hud))
	{
		ACHUD* MyHud = Cast<ACHUD>(Hud);
		if (IsValid(MyHud))
		{
			MyHud->GetHUD()->SetVisibility(ESlateVisibility::Visible);
		}
	}
	UE_LOG(LogTemp, Display, TEXT("Change to Game Input Mode"));
}
