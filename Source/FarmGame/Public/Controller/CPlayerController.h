#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "CPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ACFarmField;
class ACBase_Crop;
class UCWarningWidget;
class UCDragIconWidget;
class UCFarmFieldWidget;
class UCCropWidget;
class UCStateComponent;
class UCOptionComponent;
class UCFarmInteractionContainerWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuickSlotSelected, int32, QuickSlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDragIconShowing, bool, IsOn);

UCLASS()
class FARMGAME_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()

	ACPlayerController();

protected:
	// Execute When Poseess
	virtual void OnPossess(APawn* aPawn) override;
	// Execute Whe Player State Changed
	virtual void OnRep_PlayerState() override;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent();

public:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void OnInteract(const FInputActionValue& Value);
	void OnActionInteract(const FInputActionValue& Value);
	void Scroll(const FInputActionValue& Value);

	void OpenInventory(const FInputActionValue& Value);
	void CloseInventory(const FInputActionValue& Value);

	void ShowWidget(UUserWidget* InWidget);
	void HideWidget(UUserWidget* InWidget);

	void SetUIInputMode();
	void SetGameInputMode();

	void OnQuickSlotSelected(int32 InIndex);
	
	bool IsSlotChangable() { return bChangable; }
	void SetSlotChangable();
	void SetUnSlotChangable();

	// Widget
	// Widget - Warning
	void ShowWarningWidget(FString Message);
	UFUNCTION()
	void HideWarningWidget();

	// Widget - DragIcon
	void StartDragging(UTexture2D* ItemIcon);
	void StopDragging();
	void UpdateDragIconPosition(FVector2D NewPosition);

	// Widget - FarmField
	void ShowFarmWidget(ACFarmField* TargetField);
	UFUNCTION()
	void HideFarmWidget();

	// Switch Camera
	void SwitchCamera(AActor* TargetCamera);
	void ResetCamera();
	float GetCameraMoveTime() { return CameraMoveTime; }

	// Components
	FORCEINLINE UCStateComponent* GetStateComponent() const { return StateComp; }
	FORCEINLINE UCOptionComponent* GetOptionComponent() const { return OptionComp; }

	UFUNCTION()
	void OnStateChanged(EStateType PrevType, EStateType NewType);

private:
	void RebindAction();
	void InputTest(const FInputActionValue& Value);

	void Test1(const FInputActionValue& Value);
	void Test2(const FInputActionValue& Value);
	void Test3(const FInputActionValue& Value);
	void Test4(const FInputActionValue& Value);

public:
	UPROPERTY(BlueprintAssignable)
	FOnQuickSlotSelected OnQuickSlotSelectedDelegate; 
	
	UPROPERTY(BlueprintAssignable)
	FDragIconShowing OnDragIconShowing;

protected:
	// Component
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCStateComponent* StateComp;
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCOptionComponent* OptionComp;

	// Input
	// For IMc Check
	UPROPERTY(EditDefaultsOnly, Category = "Input|Context")
	UInputMappingContext* CurrentContext;

	// Player Move Input
	UPROPERTY(EditDefaultsOnly, Category = "Input|DefaultAction")
	UInputMappingContext* DefaultContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input|DefaultAction")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input|DefaultAction")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input|DefaultAction")
	UInputAction* InteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input|DefaultAction")
	UInputAction* ActionInteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input|DefaultAction")
	UInputAction* ScrollAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input|DefaultAction")
	UInputAction* OpenInventoryAction;

	// UI Mode Input
	UPROPERTY(EditDefaultsOnly, Category = "Input|Context")
	UInputMappingContext* UIContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input|UIAction")
	UInputAction* CloseInventoryAction;
	
	// Common Input Action
	UPROPERTY(EditDefaultsOnly, Category = "Input|CommonAction")
	TArray<UInputAction*> QuickSlotActions;
	UPROPERTY(EditDefaultsOnly, Category = "Input|CommonAction")
	TArray<UInputAction*> TestActions;
	UPROPERTY(EditAnywhere, Category = "Input|CommonAction")
	bool bChangable;

	// Widget
	UPROPERTY(VisibleAnywhere, Category="Widget")
	int32 OpenWidgetCnt;

	// Widget - Warning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget|Warning")
	TSubclassOf<UCWarningWidget> WarningWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget|Warning")
	UCWarningWidget* WarningWidget;

	// Widget - DragIcon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget|DragIcon")
	TSubclassOf<UCDragIconWidget> DragIconWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget|DragIcon")
	UCDragIconWidget* DragIconWidget;
	bool IsDragging;

	// Widget - FarmField & Crop
	UPROPERTY(VisibleAnywhere, Category = "Widget|Farm")
	TSubclassOf<UCFarmInteractionContainerWidget> FarmWidgetClass;
	UPROPERTY(VisibleAnywhere, Category = "Widget|Farm")
	UCFarmInteractionContainerWidget* FarmWidget;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	float CameraMoveTime;
};
