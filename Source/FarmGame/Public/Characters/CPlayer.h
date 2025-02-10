#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "Interfaces/CInterface_Interactable.h"
#include "Interfaces/CTestInterface.h"
#include "CPlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UCStateComponent;
class UCAttributeComponent;
class UCOptionComponent;
class USphereComponent;
class ICItemInterface;

UCLASS()
class FARMGAME_API ACPlayer : public ACharacter, public ICInterface_Interactable,public ICTestInterface
{
	GENERATED_BODY()

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Interaction", meta = (DisplayName = "Add Interactable Object"))
	void AddInteractableObject(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Interaction", meta = (DisplayName = "Remove Interactable Object"))
	void RemoveInteractableObject(AActor* InActor);

	UFUNCTION(BlueprintPure)
	FORCEINLINE TScriptInterface<ICItemInterface> const GetCurrentSlotItem() const { return ItemContainer[ItemIndex]; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE TArray<TScriptInterface<ICItemInterface>> GetItemContainer() const { return ItemContainer; }

public:
	// Inherited via ICInterface_Interactable
	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	virtual bool IsInteractable() override { return bInteractable; }

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	virtual void SetInteractable() override;

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	virtual void SetUnInteractable() override;

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	virtual void SetType(EInteractObjectType InNewType) override;

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	EInteractObjectType GetType() override { return InteractType; }

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	virtual FName GetInteractName() override { return "Player"; }

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	virtual void Interact(AActor* OtherActor) override;

	UFUNCTION(BlueprintCallable, Category = "InteractSystem")
	void DetectInteractableObjects();

	UFUNCTION(BlueprintCallable, Category = "InteractSystem")
	bool SetDetectInterval(float InTime);
	UFUNCTION(BlueprintCallable, Category = "InteractSystem")
	float GetDetectInterval() { return DetectInterval; }

	UFUNCTION(BlueprintCallable, Category = "InteractSystem")
	bool SetDetectRange(float InRange);
	UFUNCTION(BlueprintCallable, Category = "InteractSystem")
	float GetDetectRange() { return DetectRange; }

	UFUNCTION(BlueprintCallable, Category = "InteractSystem")
	bool SetRemoveRange(float InRange);
	UFUNCTION(BlueprintCallable, Category = "InteractSystem")
	float GetRemoveRange() { return RemoveRange; }

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void OnInteract(const FInputActionInstance& InInstance);
	void Scroll(const FInputActionValue& Value);

private:
	bool Trace(ECollisionChannel TraceChannel);

protected:
	// Input
	UPROPERTY(VisibleAnywhere, Category="Input")
	UInputMappingContext* DefaultContext;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	UInputAction* ScrollAction;

	// Components
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

	// Interact
	UPROPERTY(EditAnywhere, Category="Interact")
	//TArray<TScriptInterface<ICInterface_Interactable>> InteractableObjects;
	TArray<AActor*> InteractableObjects;

	UPROPERTY(EditAnywhere, Category = "Interact")
	int32 InteractIndex;

	// Item
	UPROPERTY(VisibleAnywhere, Category="Item")
	TArray<TScriptInterface<ICItemInterface>> ItemContainer;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 ItemContainerSize;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 ItemIndex;

private:
	// Trace
	FTimerHandle DetectTimer;
	float DetectInterval;
	float DetectRange;
	float RemoveRange;

	// Scroll delay
	float LastScrollTime = 0.0f;
	const float ScrollCooldown = 0.2f;
};
