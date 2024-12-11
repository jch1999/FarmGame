#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CCultivationComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCultivationChanged);

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
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetNowRate() { return CurrentCultivation / MaxCultivation; }
	
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
	UPROPERTY(EditAnywhere, Category = "Moisture")
	float CurrentCultivation;

	UPROPERTY(EditAnywhere, Category = "Moisture")
	float AutoReduceAmount;

	UPROPERTY(EditAnywhere, Category = "Moisture")
	FVector2D CultivationRange;

private:
	FTimerHandle CultivationReduceTimer;
	float MaxCultivation;
};
