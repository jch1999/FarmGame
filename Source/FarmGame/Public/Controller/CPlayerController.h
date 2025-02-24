#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

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
	void OnInteract();
	void OnActionInteract();
	void Scroll(const FInputActionValue& Value);
	void OpenInventory();

	void CloseInventory();

	void SetUIInputMode();
	void SetGameInputMode();

protected:
	// Input
	// Player Move Input
	UPROPERTY(VisibleAnywhere, Category = "Input")
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
	UPROPERTY(VisibleAnywhere, Category = "Input")
	UInputMappingContext* UIContext;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	UInputAction* CloseInventoryAction;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	UInputAction* OpenUIAction;
};
