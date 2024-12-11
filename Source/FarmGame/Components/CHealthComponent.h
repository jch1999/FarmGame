#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMGAME_API UCHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCHealthComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentHealth() { return CurrentHealth; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentRate() { return CurrentHealth / MaxHealth; }


	void SetMaxHealth(float InMaxHealth, bool bResetCurrentHealth = false);

	void IncreaseHealth(float InAmount);
	void DecreaseHealth(float InAmount);

public:
	UPROPERTY(BlueprintAssignable)
	FHealthChanged OnHealthChanged;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Health")
	float MaxHealth;

private:
	float CurrentHealth;
		
};
