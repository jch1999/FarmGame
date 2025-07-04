#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CGenericStateComponent.h"
#include "CHealthComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FARMGAME_API UCHealthComponent : public UCGenericStateComponent
{
	GENERATED_BODY()

public:
	UCHealthComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentHealth() { return GetCurrentValue(); }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMaxHealth() { return GetMaxValue(); }


	void SetMaxHealth(float InMaxHealth, bool bResetCurrentHealth = false);

	void IncreaseHealth(float InAmount);
	void DecreaseHealth(float InAmount);
	bool IsDead();
};