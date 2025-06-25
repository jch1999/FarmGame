#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/CInterface_Interactable.h"
#include "Components/CInventoryComponent.h"
#include "CPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCStateComponent;
class UCAttributeComponent;
class UCOptionComponent;
class USphereComponent;
class UCInteractComponent;
class ICItemInterface;
class UAnimMontage;
class UCQuickSlotBarWidget;
class ACItemBase;
class UMaterialInstanceDynamic;
class UPostProcessComponent;

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
	virtual void SetDelayedInteractable(float DelayTime) override;

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	virtual void SetDelayedUninteractable(float DelayTime) override;

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	EInteractObjectType GetType() override { return InteractType; }
	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	virtual FName GetInteractName() override { return "Player"; }
	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	void SetType(EInteractObjectType InNewType) override;
	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	void Interact(AActor* OtherActor = nullptr) override;
	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	bool OnHovered() override;
	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	bool OnUnhovered() override;

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface|Custom")
	void ActionInteract();

	ACItemBase* GetCurretnEquippedItem();

	// Get Components
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComp; }
	//FORCEINLINE UCOptionComponent* GetOptionComponent() const { return OptionComp; }
	FORCEINLINE UCInteractComponent* GetInteractComponent() const { return InteractComp; }
	FORCEINLINE UCInventoryComponent* GetInventoryComponent() const { return InventoryComp; }

	// Animation
	void StartPlantingAnimation();
	void OnPlantingAnimationFinished();
	void StartWateringAnimation();
	void OnWateringAnimationFinished();

	// Item
	UCQuickSlotBarWidget* GetQuickSlotBar();
	void EquipItemFromQuickSlot(int32 QuickSlotIndex);

	// Fade
	void StartFade(bool bToTransparent);

private:
	void UpdateFade();

protected:
	// Components
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCameraComponent* CameraComp;

	//UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	//UCStateComponent* StateComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCAttributeComponent* AttributeComp;

	//UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	//UCOptionComponent* OptionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCInteractComponent* InteractComp;
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCInventoryComponent* InventoryComp;

	//UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	//UPostProcessComponent* PostProcessComp;

	// Interact Interface
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractInterface")
	bool bInteractable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractInterface")
	EInteractObjectType InteractType;

	// Fade
	UPROPERTY()
	UMaterialInstanceDynamic* FadeMaterialInstance;

	FTimerHandle OpacityTimerHandle;
	float TargetOpacity;
	float CurrentOpacity;
	float FadeLerpDuration;
	float FadeLerpElapsed;
	bool bFadingOut;

private:
	// Animation
	UPROPERTY(EditDefaultsOnly,Category="Animation")
	UAnimMontage* PlantAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* WateringAnim;

	// EuqipedItem
	UPROPERTY(VisibleAnywhere, Category="Item")
	ACItemBase* CurrentEquippedItem;

	FTimerHandle InteractTimer;
};
