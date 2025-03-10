#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "CPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuickSlotSelected, int32, QuickSlotIndex);

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
	
	UFUNCTION()
	void CloseInventoryForCloseBtn();

	void SetUIInputMode();
	void SetGameInputMode();

	void OnQuickSlotSelected(int32 InIndex);

private:
	void RebindAction();
	void InputTest(const FInputActionValue& Value);

public:
	UPROPERTY(BlueprintAssignable)
	FOnQuickSlotSelected OnQuickSlotSelectedDelegate;

protected:
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

	UPROPERTY(VisibleDefaulsOnly, Category="Widget")
	int32 OpenWigetCnt;
};
