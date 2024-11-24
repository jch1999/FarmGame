#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CCultivationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMGAME_API UCCultivationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCCultivationComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE float GetCultivationValue() { return NowCultivation; }

	void AddCultivation(float InAmount);
	void ReduceCultivation(float InAmount);
	void SetCultivationRange(FVector2D InNewRange);
	void SetAutoReduceAmount(float InReduceAmount);

	void SetAutoReduceTimer(float InFirstDelay, bool InbLoop = false, float InLoopDelay = 0.0f);

private:
	void AutoReduceCultivation();

protected:
	UPROPERTY(EditAnywhere, Category = "Moisture")
	float NowCultivation;

	UPROPERTY(EditAnywhere, Category = "Moisture")
	float AutoReduceAmount;

	UPROPERTY(EditAnywhere, Category = "Moisture")
	FVector2D CultivationRange;

private:
	FTimerHandle CultivationReduceTimer;
};
