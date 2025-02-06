#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CCultivationComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCultivationChanged, float, OldValue, float, NewValue,float, MaxValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMGAME_API UCCultivationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCCultivationComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentCultivation() { return CurrentCultivation; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMaxCultivation() { return MaxCultivation; }
	
	void AddCultivation(float InAmount);
	void ReduceCultivation(float InAmount);
	void SetCultivationRange(FVector2D InNewRange);
	void SetAutoReduceAmount(float InReduceAmount);

	void SetAutoReduceTimer(float InFirstDelay, bool InbLoop = false, float InLoopDelay = 0.0f);

private:
	void AutoReduceCultivation();

public:
	UPROPERTY(BlueprintAssignable)
	FCultivationChanged OnCultivationChanged;

protected:
	UPROPERTY(EditAnywhere, Category = "Cultivation")
	float CurrentCultivation;

	UPROPERTY(EditAnywhere, Category = "Cultivation")
	float AutoReduceAmount;

	UPROPERTY(EditAnywhere, Category = "Cultivation")
	FVector2D CultivationRange;

private:
	FTimerHandle CultivationReduceTimer;
	float MaxCultivation;
};
