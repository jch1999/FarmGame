#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CGenericStateComponent.h"
#include "CNutritionComponent.generated.h"

UENUM(BlueprintType)
enum class ENutritionState :uint8
{
	Famine, Enough, Over
};

// Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNutritionStateChanged, ENutritionState, InNewState);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMGAME_API UCNutritionComponent : public UCGenericStateComponent
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

	void SetFamineState();
	void SetEnoughState();
	void SetOverState();

	UFUNCTION(BlueprintPure)
	FORCEINLINE ENutritionState GetCurrentState() { return NutritionState; }

	void AddNutrition(float InAmount);
	void ReduceNutrition(float ImAmount);
	virtual void SetSafeRange(FVector2D InNewRange) override;
	
private:
	virtual void AutoReduceValue() override;

	void CheckState();
	void ChangeState(ENutritionState state);

public:
	UPROPERTY(BlueprintAssignable)
	FNutritionStateChanged OnNutritionStateChanged;

private:
	ENutritionState NutritionState;
};
