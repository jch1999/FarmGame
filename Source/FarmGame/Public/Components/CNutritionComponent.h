#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CNutritionComponent.generated.h"

UENUM(BlueprintType)
enum class ENutritionState :uint8
{
	Famine, Enough, Over
};

// Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNutritionStateChanged, ENutritionState, InNewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FNutritionChanged, float, OldValue, float, NewValue, float, MaxValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMGAME_API UCNutritionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCNutritionComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE bool IsEnough() { return NutritionState == ENutritionState::Enough; }
	FORCEINLINE bool IsFamine() { return NutritionState == ENutritionState::Famine; }
	FORCEINLINE bool ISOver() { return NutritionState == ENutritionState::Over; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE FVector2D GetSafeRange() { return NutritionSafeRange; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMaxNutrition() { return MaxNutrition; }
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentNutrition() { return CurrentNutrition; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentRate() { return CurrentNutrition / MaxNutrition; }

	void SetFamineState();
	void SetEnoughState();
	void SetOverState();

	void AddNutrition(float InAmount);
	void ReduceNutrition(float ImAmount);
	void SetSafeRange(FVector2D InNewRange);
	void SetAutoReduceAmount(float InReduceAmount);

	void SetAutoReduceTimer(float InFirstDelay, bool InbLoop = false, float InLoopDelay = 0.0f);

private:
	void AutoReduceNutirition();

	void CheckState();
	void ChangeState(ENutritionState state);

public:
	UPROPERTY(BlueprintAssignable)
	FNutritionStateChanged OnNutritionStateChanged;

	UPROPERTY(BlueprintAssignable)
	FNutritionChanged OnNutritionChanged;

protected:
	UPROPERTY(EditAnywhere, Category = "Nutrition")
	float CurrentNutrition;

	UPROPERTY(EditDefaultsOnly, Category = "Nutrition")
	float MaxNutrition;

	UPROPERTY(EditAnywhere, Category = "Nutrition")
	float AutoReduceAmount;

	UPROPERTY(EditAnywhere, Category = "Nutrition")
	FVector2D NutritionSafeRange;

private:
	FTimerHandle NutiritionReduceTimer;
	ENutritionState NutritionState;
};
