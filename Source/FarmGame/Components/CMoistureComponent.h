#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMoistureComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMGAME_API UCMoistureComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCMoistureComponent();

protected:
	virtual void BeginPlay() override;

public:	
	FORCEINLINE bool IsEnough() { return (MoistureSafeRange.X <= NowMoisture) && (MoistureSafeRange.Y >= NowMoisture); }
	FORCEINLINE bool IsUnder() { return NowMoisture < MoistureSafeRange.X; }
	FORCEINLINE bool ISOver() { return NowMoisture > MoistureSafeRange.Y; }
	FORCEINLINE FVector2D GetSafeRange() { return MoistureSafeRange; }
	FORCEINLINE float GetMoistureValue() { return NowMoisture; }

	void AddMoisture(float Amount);
	void ReduceMoisture(float Amount);
	void SetSafeRange(FVector2D NewRange);
	void SetAutoReduceAmount(float InReduceAmount);

	UFUNCTION(BlueprintCallable)
	void SetAutoReduceTimer(float InFirstDelay, bool InbLoop = false, float InLoopDelay = 0.0f);

private:
	void AutoReduceMoisture();

protected:
	UPROPERTY(EditAnywhere, Category="Moisture")
	float NowMoisture;

	UPROPERTY(EditAnywhere, Category = "Moisture")
	float AutoReduceAmount;

	UPROPERTY(EditAnywhere, Category = "Moisture")
	FVector2D MoistureSafeRange;

private:
	FTimerHandle MoistureReduceTimer;
};
