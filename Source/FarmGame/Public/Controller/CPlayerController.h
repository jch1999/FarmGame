#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "CPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

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
	UFUNCTION()
	void OpenInventory(const FInputActionValue& Value);

	UFUNCTION()
	void CloseInventory(const FInputActionValue& Value);
	
	UFUNCTION()
	void CloseInventoryForCloseBtn();

	void SetUIInputMode();
	void SetGameInputMode();

private:
	void RebindAction();

protected:
	// Input
	// For IMc Check
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* CurrentContext;

	// Player Move Input
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultContext;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	UInputAction* ActionInteractAction;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	UInputAction* ScrollAction;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	UInputAction* OpenInventoryAction;

	// UI Mode Input
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* UIContext;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	UInputAction* CloseInventoryAction;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	UInputAction* OpenUIAction;
};
