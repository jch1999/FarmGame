#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CGenericStateComponent.h"
#include "CHealthComponent.generated.h"

UENUM(BlueprintType)
enum class EHealthState :uint8
{
	Dead, Deadly, Sick, Healthy
};

// Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthStateChanged, EHealthState, InNewState);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FARMGAME_API UCHealthComponent : public UCGenericStateComponent
{
	GENERATED_BODY()

public:
	UCHealthComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE bool IsHealthy() { return HealthState == EHealthState::Healthy; }
	FORCEINLINE bool IsSick() { return HealthState == EHealthState::Sick; }
	FORCEINLINE bool IsDeadly() { return HealthState == EHealthState::Deadly; }
	FORCEINLINE bool IsDead() { return HealthState == EHealthState::Dead; }

	void SetHealthyState();
	void SetSickState();
	void SetDeadlyState();
	void SetDeadState();

	UFUNCTION(BlueprintPure)
	FORCEINLINE EHealthState GetCurrentState() { return HealthState; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentHealth() { return GetCurrentValue(); }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMaxHealth() { return GetMaxValue(); }


	void SetMaxHealth(float InMaxHealth, bool bResetCurrentHealth = false);

	void IncreaseHealth(float InAmount);
	void DecreaseHealth(float InAmount);

private:
	void CheckState();
	void ChangeState(EHealthState state);

public:
	UPROPERTY(BlueprintAssignable)
	FHealthStateChanged OnHealthStateChanged;

private:
	EHealthState HealthState;
};