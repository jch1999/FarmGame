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
	void Interact(AActor* OtherActor = nullptr) override;
	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	bool OnHovered() override;
	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	bool OnUnhovered() override;

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface|Custom")
	void ActionInteract();
	
	// Camera Move
	void MoveCameraToLocation(const FVector& TargetLocation, const FRotator& TargetRotation);
	void RestoreCamera();

	// Get Components
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComp; }
	FORCEINLINE UCOptionComponent* GetOptionComponent() const { return OptionComp; }
	FORCEINLINE UCInteractComponent* GetInteractComponent() const  { return InteractComp; }
	FORCEINLINE UCInventoryComponent* GetInventoryComponent() const { return InventoryComp; }
	
private:
	void UpdateCameraLerp();

protected:
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
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCInventoryComponent* InventoryComp;

	// Interact Interface
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractInterface")
	bool bInteractable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractInterface")
	EInteractObjectType InteractType;

	// Temp Inventory
	UPROPERTY(VisibleAnywhere, Category="Item")
	TArray<TScriptInterface<ICItemInterface>> ItemContainer;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 ItemContainerSize;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 ItemIndex;

private:
	FVector OriginalCameraRelativeLocation;
	FRotator OriginalCameraRelativeRotation; 
	
	FTimerHandle CameraLerpTimerHandle;
	float CameraLerpAlpha = 0.0f;

	FVector CameraStartLocation;
	FRotator CameraStartRotation;
	FVector CameraTargetLocation;
	FRotator CameraTargetRotation;
};
