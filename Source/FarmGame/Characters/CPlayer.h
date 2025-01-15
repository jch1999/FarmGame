#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "Interfaces/CInterface_Interactable.h"
#include "CPlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UCStateComponent;
class UCAttributeComponent;
class UCOptionComponent;
class ICItemInterface;

UCLASS()
class FARMGAME_API ACPlayer : public ACharacter, public ICInterface_Interactable
{
	GENERATED_BODY()

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE TScriptInterface<ICItemInterface> const GetCurrentSlotItem() const { return ItemContainer[ItemIndex]; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE TArray<TScriptInterface<ICItemInterface>> GetItemContainer() const { return ItemContainer; }

protected:
	// Inherited via ICInterface_Interactable
	bool IsInteractable() override;
	void SetInteractable() override;
	void SetUnInteractable() override;
	void Interact() override;

	EInteractObjectType GetType() override;
	void SetType(EInteractObjectType InNewType) override;

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void OnInteract(const FInputActionInstance& InInstance);

private:
	bool Trace(const ICInterface_Interactable* OutInteract);

protected:
	UPROPERTY(VisibleAnywhere, Category="Input")
	UInputMappingContext* DefaultContext;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCStateComponent* StateComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCAttributeComponent* AttributeComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCOptionComponent* OptionComp;

	UPROPERTY(VisibleAnywhere, Category="Item")
	TArray<TScriptInterface<ICItemInterface>> ItemContainer;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 ItemContainerSize;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 ItemIndex;
};
