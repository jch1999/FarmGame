#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CActionComponent.h"
#include "CAnimInstance.generated.h"

UCLASS()
class FARMGAME_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeBeginPlay() override;

private:
	UFUNCTION()
	void OnActionTypeChanged(EActionType InPrevType, EActionType InNewType);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Player")
	float Speed;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Player")
	float Direction;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Action")
	EActionType ActionType;
};
