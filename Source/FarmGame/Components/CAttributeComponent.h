#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMGAME_API UCAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCAttributeComponent();

public:
	FORCEINLINE float GetSneakSpeed() { return SneakSpeed; }
	FORCEINLINE float GetWalkSpeed() { return WalkSpeed; }
	FORCEINLINE float GetSprintSpeed() { return SprintSpeed; }
	FORCEINLINE bool IsCanMove() { return bCanMove; }

	void SetMove();
	void SetStop();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Speed")
	float SneakSpeed;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Speed")
	float WalkSpeed;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Speed")
	float SprintSpeed;

private:
	bool bCanMove;
};
