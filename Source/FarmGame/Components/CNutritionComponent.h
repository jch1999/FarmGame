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

	void AddNutrition(float Amount);
	void SetSafeRange(FVector2D NewRange);
protected:
	UPROPERTY(EditAnywhere, Category = "Moisture")
	float NowNutrition;

	UPROPERTY(EditAnywhere, Category = "Moisture")
	FVector2D NutritionSafeRange;

private:
	FTimeHandle NutiritionReduceTimer;
};
