#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMGAME_API UCHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCHealthComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE float GetCurrentHealth() { return CurrentHealth; }
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }

	void SetMaxHealth(float InMaxHealth, bool bResetCurrentHealth = false);

	void IncreaseHealth(float InAmount);
	void DecreaseHealth(float InAmount);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Health")
	float MaxHealth;

private:
	float CurrentHealth;
		
};
