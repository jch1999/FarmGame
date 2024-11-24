#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CNutritionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMGAME_API UCNutritionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCNutritionComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE bool IsEnough() { return (NutritionSafeRange.X <= NowNutrition) && (NutritionSafeRange.Y >= NowNutrition); }
	FORCEINLINE bool IsUnder() { return NowNutrition < NutritionSafeRange.X; }
	FORCEINLINE bool ISOver() { return NowNutrition > NutritionSafeRange.Y; }
	FORCEINLINE FVector2D GetSafeRange() { return NutritionSafeRange; }
	FORCEINLINE float GetNutritionValue() { return NowNutrition; }
	
	void AddNutrition(float InAmount);
	void ReduceNutrition(float ImAmount);
	void SetSafeRange(FVector2D InNewRange);
	void SetAutoReduceAmount(float InReduceAmount);

	void SetAutoReduceTimer(float InFirstDelay, bool InbLoop = false, float InLoopDelay = 0.0f);

private:
	void AutoReduceNutirition();

protected:
	UPROPERTY(EditAnywhere, Category = "Moisture")
	float NowNutrition;

	UPROPERTY(EditAnywhere, Category = "Moisture")
	float AutoReduceAmount;

	UPROPERTY(EditAnywhere, Category = "Moisture")
	FVector2D NutritionSafeRange;

private:
	FTimerHandle NutiritionReduceTimer;
};
