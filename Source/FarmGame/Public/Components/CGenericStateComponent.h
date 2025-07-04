#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CGenericStateComponent.generated.h"

// Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FStateValueChanged, float, OldValue, float, NewValue, float, MaxValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMGAME_API UCGenericStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCGenericStateComponent();

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintPure)
	FORCEINLINE FVector2D GetSafeRange() { return SafeRange; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMaxValue() { return MaxValue; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentValue() { return CurrentValue; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentRate() { return CurrentValue / MaxValue; }
	
	void AddValue(float InAmount);
	void ReduceValue(float ImAmount);
	
	virtual void SetSafeRange(FVector2D InNewRange);
	void SetAutoReduceAmount(float InReduceAmount);

	void SetAutoReduceTimer(float InFirstDelay, bool InbLoop = false, float InLoopDelay = 0.0f);

protected:
	virtual void AutoReduceValue();

public:
	UPROPERTY(BlueprintAssignable)
	FStateValueChanged OnStateValueChanged;

protected:
	UPROPERTY(EditAnywhere, Category = "Status")
	float CurrentValue;

	UPROPERTY(EditAnywhere, Category = "Status")
	float MaxValue;

	UPROPERTY(EditAnywhere, Category = "Status")
	float AutoReduceAmount;

	UPROPERTY(EditAnywhere, Category = "Status")
	FVector2D SafeRange;

private:
	FTimerHandle AutoReduceTimer;
};
