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
class USphereComponent;
class UCInteractComponent;
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
	// ICInterface_Interactable을(를) 통해 상속됨
	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	FORCEINLINE bool IsInteractable() override { return bInteractable; }
	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	void SetInteractable() override;
	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	void SetUnInteractable() override;
	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	EInteractObjectType GetType() override { return InteractType; }
	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	virtual FName GetInteractName() override { return "Player"; }
	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	void SetType(EInteractObjectType InNewType) override;
	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	void Interact(AActor* OtherActor) override;

public:
	FORCEINLINE UCameraComponent* GetCameraComponent() { return CameraComp; }
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE TScriptInterface<ICItemInterface> const GetCurrentSlotItem() const { return ItemContainer[ItemIndex]; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE TArray<TScriptInterface<ICItemInterface>> GetItemContainer() const { return ItemContainer; }

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void OnInteract(const FInputActionInstance& InInstance);
	void Scroll(const FInputActionValue& Value);

private:
	bool Trace(ECollisionChannel TraceChannel, TArray<FHitResult>& Hits);

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

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCInteractComponent* InteractComp;

	// Item
	UPROPERTY(VisibleAnywhere, Category="Item")
	TArray<TScriptInterface<ICItemInterface>> ItemContainer;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 ItemContainerSize;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 ItemIndex;
};
