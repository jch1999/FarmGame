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
#include "UI/CWarningWidget.h"
#include "UI/CDragIconWidget.h"
#include "UI/CFarmFieldWidget.h"
#include "UI/CCropWidget.h"
#include "UI/CQuickSlotBarWidget.h"
#include "Components/Button.h"
#include "CGameInstance.h"

ACPlayerController::ACPlayerController()
{
	// Input Setting
	// Default
	CHelpers::GetAsset(&DefaultContext, "/Game/Input/IMC_Player");
	CHelpers::GetAsset(&MoveAction, "/Game/Input/IA_PlayerMove");
	CHelpers::GetAsset(&LookAction, "/Game/Input/IA_PlayerRotate");
	CHelpers::GetAsset(&InteractAction, "/Game/Input/IA_PlayerInteract");
	CHelpers::GetAsset(&ActionInteractAction, "/Game/Input/IA_PlayerActionInteract");
	CHelpers::GetAsset(&ScrollAction, "/Game/Input/IA_PlayerScroll");
	CHelpers::GetAsset(&OpenInventoryAction, "/Game/Input/IA_OpenInventory");
	// UI
	CHelpers::GetAsset(&UIContext, "/Game/Input/IMC_UI");

	// Common
	QuickSlotActions.SetNum(6);

	for (int32 i = 0; i < 6; i++)
	{
		FString AssetPath = FString::Printf(TEXT("/Game/Input/IA_QuickSlot%d"), i + 1);
		CHelpers::GetAsset(&QuickSlotActions[i], *AssetPath);
	}
	TestActions.SetNum(4);
	for (int32 i = 0; i < 4; i++)
	{
		FString AssetPath = FString::Printf(TEXT("/Game/Input/IA_Test%d"), i + 1);
		CHelpers::GetAsset(&TestActions[i], *AssetPath);
	}

	// UI Class
	CHelpers::GetAsset(&CloseInventoryAction, "/Game/Input/IA_CloseInventory");
	if (!WarningWidgetClass)
	{
		CHelpers::GetClass(&WarningWidgetClass, "/Game/UI/WB_WarningWidget");
	}
	if(!DragIconWidgetClass)
	{
		CHelpers::GetClass(&DragIconWidgetClass, "/Game/UI/WB_DragIcon");
	}
	if (!FarmFieldWidgetClass)
	{
		CHelpers::GetClass<UCFarmFieldWidget>(&FarmFieldWidgetClass, "/Game/UI/WB_FarmFieldWidget");
	}
	if (!CropWidgetClass)
	{
		CHelpers::GetClass<UCCropWidget>(&CropWidgetClass, "/Game/UI/WB_CropWidget");
	}

	CameraMoveTime = 1.5f;
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
			MyHud->CreateHUD(this);
			ACPlayer* MyPlayer = Cast<ACPlayer>(aPawn);
			if (MyPlayer)
			{
				MyHud->GetHUD()->OnInitPlayer(MyPlayer);
			}
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
			MyHud->CreateHUD(this);
			ACPlayer* MyPlayer = Cast<ACPlayer>(GetPawn());
			if (MyPlayer)
			{
				MyHud->GetHUD()->OnInitPlayer(MyPlayer);
			}
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
		CurrentContext = DefaultContext;
	}
}

void ACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (!CurrentContext)
	{
		CurrentContext = DefaultContext;
	}
	RebindAction();
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
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Player Missing at InteractInput!"));
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
		MyPlayer->GetInventoryComponent()->ShowInventoryWidget();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Player. Function : ACPlayerController::OpenInventory"));
	}
}

void ACPlayerController::CloseInventory(const FInputActionValue& Value)
{
	ACPlayer* MyPlayer = Cast<ACPlayer>(GetPawn());
	if (MyPlayer)
	{
		MyPlayer->GetInventoryComponent()->HideInventoryWidget();
		UE_LOG(LogTemp, Error, TEXT("Function : ACPlayerController::CloseInventory"))
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Player. Function : ACPlayerController::CloseInventory"));
	}
}

void ACPlayerController::SetUIInputMode()
{
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
	bShowMouseCursor = true;

	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (SubSystem)
	{
		SubSystem->RemoveMappingContext(DefaultContext);
		SubSystem->AddMappingContext(UIContext, 1);

		CurrentContext = UIContext;
		RebindAction();
	}

	// Hide Hud
	AHUD* Hud = GetHUD();
	if (IsValid(Hud))
	{
		ACHUD* MyHud = Cast<ACHUD>(Hud);
		if (IsValid(MyHud))
		{
			MyHud->GetHUD()->OnUIMode();
		}
	}

	UE_LOG(LogTemp, Display, TEXT("Change to UI Input Mode")); 
	UE_LOG(LogTemp, Warning, TEXT("CurrentContext in SetUIInputMode: %s"), *CurrentContext->GetFName().ToString());

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
		CurrentContext = DefaultContext;
		RebindAction();
	}

	// Show Hud
	AHUD* Hud = GetHUD();
	if (IsValid(Hud))
	{
		ACHUD* MyHud = Cast<ACHUD>(Hud);
		if (IsValid(MyHud))
		{
			MyHud->GetHUD()->OnGameMode();
		}
	}
	UE_LOG(LogTemp, Display, TEXT("Change to Game Input Mode"));
}

void ACPlayerController::OnQuickSlotSelected(int32 InIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("QuickSlot %d Selected"), InIndex);

	OnQuickSlotSelectedDelegate.Broadcast(InIndex);
}

void ACPlayerController::SwitchCamera(AActor* TargetCamera)
{
	if (!TargetCamera) return;

	SetViewTargetWithBlend(TargetCamera, CameraMoveTime, EViewTargetBlendFunction::VTBlend_Cubic, 0.5f, false);
}

void ACPlayerController::ResetCamera()
{
	if (APawn* PossessedPawn = GetPawn())
	{
		SetViewTargetWithBlend(PossessedPawn, CameraMoveTime, EViewTargetBlendFunction::VTBlend_Cubic, 0.5f, false);
	}
}


void ACPlayerController::RebindAction()
{
	UE_LOG(LogTemp, Warning, TEXT("Rebinding Actions in Context: %s"), *CurrentContext->GetFName().ToString());

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->ClearActionBindings();

		if (CurrentContext && CurrentContext->GetFName() == DefaultContext->GetFName())
		{
			// 게임 모드 액션 바인딩
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPlayerController::Move);
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPlayerController::Look);
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ACPlayerController::OnInteract);
			EnhancedInputComponent->BindAction(ActionInteractAction, ETriggerEvent::Started, this, &ACPlayerController::OnActionInteract);
			EnhancedInputComponent->BindAction(ScrollAction, ETriggerEvent::Started, this, &ACPlayerController::Scroll);
			EnhancedInputComponent->BindAction(OpenInventoryAction, ETriggerEvent::Started, this, &ACPlayerController::OpenInventory);
		}
		else if (CurrentContext && CurrentContext->GetFName() == UIContext->GetFName())
		{
			// UI 모드 액션 바인딩
			EnhancedInputComponent->BindAction(CloseInventoryAction, ETriggerEvent::Started, this, &ACPlayerController::CloseInventory);
			UE_LOG(LogTemp, Warning, TEXT("Checking CloseInventoryAction Bind: %s"), *CloseInventoryAction->GetFName().ToString());
		}

		for (int32 i = 0; i < 6; i++)
		{
			EnhancedInputComponent->BindAction(QuickSlotActions[i], ETriggerEvent::Started, this, &ACPlayerController::OnQuickSlotSelected, i+1);
		}

		EnhancedInputComponent->BindAction(TestActions[0], ETriggerEvent::Started, this, &ACPlayerController::Test1);
		EnhancedInputComponent->BindAction(TestActions[1], ETriggerEvent::Started, this, &ACPlayerController::Test2);
		EnhancedInputComponent->BindAction(TestActions[2], ETriggerEvent::Started, this, &ACPlayerController::Test3);
		EnhancedInputComponent->BindAction(TestActions[3], ETriggerEvent::Started, this, &ACPlayerController::Test4);
	}
}

void ACPlayerController::InputTest(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Error, TEXT("InputText! Input Works!"));
}

void ACPlayerController::Test1(const FInputActionValue& Value)
{
	ACPlayer* MyPlayer = Cast<ACPlayer>(GetPawn());
	if (MyPlayer)
	{
		MyPlayer->GetInventoryComponent()->AddMoney(100);
	}
}

void ACPlayerController::Test2(const FInputActionValue& Value)
{
	ACPlayer* MyPlayer = Cast<ACPlayer>(GetPawn());
	if (MyPlayer)
	{
		MyPlayer->GetInventoryComponent()->UseMoney(30);
	}
}

void ACPlayerController::Test3(const FInputActionValue& Value)
{
	ShowWarningWidget("Test!!!!!!!!!!!!");
}

void ACPlayerController::Test4(const FInputActionValue& Value)
{
	HideWarningWidget();
}

void ACPlayerController::StartDragging(UTexture2D* ItemIcon)
{
	if (!DragIconWidget)
	{
		DragIconWidget = CreateWidget<UCDragIconWidget>(this, DragIconWidgetClass);
		if (DragIconWidget)
		{
			DragIconWidget->SetDesiredSizeInViewport(FVector2D(30, 30));
			DragIconWidget->AddToViewport();
		}
	}
	DragIconWidget->InitDragIcon(ItemIcon);
	DragIconWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	OnDragIconShowing.Broadcast(true);
}

void ACPlayerController::StopDragging()
{
	if (DragIconWidget)
	{
		DragIconWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	OnDragIconShowing.Broadcast(false);
}

void ACPlayerController::UpdateDragIconPosition(FVector2D NewPosition)
{
	if (DragIconWidget)
	{
		DragIconWidget->SetRenderTranslation(NewPosition);
	}
}

void ACPlayerController::ShowWarningWidget(FString Message)
{
	if (!WarningWidget)
	{
		WarningWidget = CreateWidget<UCWarningWidget>(this, WarningWidgetClass);
		WarningWidget->SetDesiredSizeInViewport(FVector2D(450, 150));
		WarningWidget->AddToViewport();
	}

	ShowWidget(WarningWidget);
	WarningWidget->SetWarningText(Message);
}


void ACPlayerController::HideWarningWidget()
{
	if (WarningWidget)
	{
		HideWidget(WarningWidget);
	}
}

void ACPlayerController::ShowFarmFieldWidget(ACFarmField* TargetField)
{
	if (FarmFieldWidgetClass && !FarmFieldWidget)
	{
		FarmFieldWidget = CreateWidget<UCFarmFieldWidget>(this, FarmFieldWidgetClass);
		if (FarmFieldWidget)
		{
			FarmFieldWidget->AddToViewport();
		}
	}

	if (FarmFieldWidget)
	{
		FarmFieldWidget->SetFarmField_Implementation(TargetField);
		FarmFieldWidget->GetPlantBtn()->OnClicked.AddDynamic(this, &ACPlayerController::HideFarmFieldWidget);
		FarmFieldWidget->PositionStateDisplays();

		if (AHUD* HUD = GetHUD())
		{
			if (ACHUD* MyHud = Cast<ACHUD>(HUD))
			{
				if (UCHUDWidget* HudWidget = MyHud->GetHUD())
				{
					if (UCQuickSlotBarWidget* QuickSlotBar = HudWidget->GetQuickSlotBar())
					{
						FarmFieldWidget->CheckPlantBtnActive(QuickSlotBar->CurrentIndex);
					}
				}
			}
		}
		ShowWidget(FarmFieldWidget);
	}
}

void ACPlayerController::HideFarmFieldWidget()
{
	if (FarmFieldWidget)
	{
		FarmFieldWidget->ResetFarmField_Implementation();
		HideWidget(FarmFieldWidget);
		ResetCamera();
	}
}

void ACPlayerController::ShowCropWidget(ACBase_Crop* TargetCrop)
{
	if (CropWidgetClass && !CropWidget)
	{
		CropWidget = CreateWidget<UCCropWidget>(this, CropWidgetClass);
		if (CropWidget)
		{
			CropWidget->AddToViewport();
		}
	}

	if (CropWidget)
	{
		CropWidget->SetCrop_Implementation(TargetCrop);
		CropWidget->GetHarvestBtn()->OnClicked.AddDynamic(this, &ACPlayerController::HideCropWidget);
		// CropWidget->PositionStateDisplays();

		ShowWidget(CropWidget);
	}
}

void ACPlayerController::HideCropWidget()
{
	if (CropWidget)
	{
		CropWidget->ResetCrop_Implementation();
		HideWidget(CropWidget);
		ResetCamera();
	}
}



void ACPlayerController::ShowWidget(UUserWidget* InWidget)
{
	if (InWidget && !(InWidget->GetVisibility()== ESlateVisibility::Visible))
	{
		InWidget->SetVisibility(ESlateVisibility::Visible);
		OpenWidgetCnt++;

		if (OpenWidgetCnt == 1)
		{
			SetUIInputMode();
		}

		UE_LOG(LogTemp, Log, TEXT("ShowWidget: %s (OpenWidgetCnt: %d)"), *InWidget->GetName(), OpenWidgetCnt);
	}
}

void ACPlayerController::HideWidget(UUserWidget* InWidget)
{
	if (InWidget && (InWidget->GetVisibility() == ESlateVisibility::Visible))
	{
		InWidget->SetVisibility(ESlateVisibility::Collapsed);
		OpenWidgetCnt = FMath::Max(0, OpenWidgetCnt - 1);

		if (OpenWidgetCnt == 0)
		{
			SetGameInputMode();
		}

		UE_LOG(LogTemp, Log, TEXT("HideWidget: %s (OpenWidgetCnt: %d)"), *InWidget->GetName(), OpenWidgetCnt);
	}
}