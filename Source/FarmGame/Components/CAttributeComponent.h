#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAttributeComponent.generated.h"

UENUM(BlueprintType)
enum class ESpeedType :uint8
{
	Sneak, Walk, Sprint, Max
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMGAME_API UCAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCAttributeComponent();

public:
	FORCEINLINE float GetSneakSpeed() { return WalkSpeeds[(int32)ESpeedType::Sneak]; }
	FORCEINLINE float GetWalkSpeed() { return WalkSpeeds[(int32)ESpeedType::Walk]; }
	FORCEINLINE float GetSprintSpeed() { return WalkSpeeds[(int32)ESpeedType::Sprint]; }
	FORCEINLINE bool IsCanMove() { return bCanMove; }

	void SetMove();
	void SetStop();
	void SetSpeed(ESpeedType InType);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Speed")
	float WalkSpeeds[(int32)ESpeedType::Max];

private:
	bool bCanMove;
};
