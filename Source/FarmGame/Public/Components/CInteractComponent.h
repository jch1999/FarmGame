#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/CInterface_Interactable.h"
#include "CInteractComponent.generated.h"

class ACHUD;
class UCHUDWidget;
class ACPlayerController;
class ACPlayer;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMGAME_API UCInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCInteractComponent();

protected:
	virtual void BeginPlay() override;

public:
	// Add / Remove InteractableObject
	UFUNCTION(BlueprintCallable, Category = "Interaction", meta = (DisplayName = "Add Interactable Object"))
	void AddInteractableObject(AActor* InActor);
	UFUNCTION(BlueprintCallable, Category = "Interaction", meta = (DisplayName = "Remove Interactable Object"))
	void RemoveInteractableObject(AActor* InActor);

	// Scroll & Select
	void Scroll(float InputValue);
	void DoInteract(AActor* OtherActor);

	// DoTrace
	void DetectInteractableObjects();
	bool RangeTrace(ECollisionChannel TraceChannel, TArray<FHitResult>& Hits);
	bool CameraTrace(ECollisionChannel TraceChannel, FHitResult& Hit);

	// Return currently seleted interact object
	AActor* GetCurrentInteractTarget();

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

	UFUNCTION(BlueprintCallable, Category = "InteractSystem")
	bool SetDetectDistance(float InDistance);
	UFUNCTION(BlueprintCallable, Category = "InteractSystem")
	float GetDetectDistance() { return DetectDistance; }

	UFUNCTION(BlueprintCallable, Category = "InteractSystem")
	bool SetRemoveDistance(float InDistance);
	UFUNCTION(BlueprintCallable, Category = "InteractSystem")
	float GetRemoveDistance() { return RemoveDistance; }

private:
	// Owner Player (이 컴포넌트가 부착된 액터)
	UPROPERTY()
	ACPlayer* OwnerCharacter;
	UPROPERTY(EditAnywhere, Category = "Interact")
	TArray<AActor*> InteractableObjects;
	AActor* ActionInteractTarget;

	// Trace
	FTimerHandle DetectTimer;

	UPROPERTY(EditAnywhere, Category = "Trace")
	float DetectInterval;

	// For Range Detect
	UPROPERTY(EditAnywhere, Category = "Trace|Range")
	float DetectRange;

	UPROPERTY(EditAnywhere, Category = "Trace|Range")
	float RemoveRange;

	// For CameraDetect
	UPROPERTY(EditAnywhere, Category = "Trace|Camera")
	float DetectDistance;

	UPROPERTY(EditAnywhere, Category = "Trace|Camera")
	float RemoveDistance;

	// Scroll delay
	float LastScrollTime;
	UPROPERTY(EditAnywhere, Category = "Scroll")
	float ScrollCooldown;
};
