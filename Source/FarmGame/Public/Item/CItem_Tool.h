#pragma once

#include "CoreMinimal.h"
#include "Item/CItemBase.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "CItem_Tool.generated.h"

class ACPlayer;

UCLASS()
class FARMGAME_API ACItem_Tool : public ACItemBase
{
	GENERATED_BODY()

public:
	ACItem_Tool();

public:
	// From Interface
	virtual bool UseItem() override;
	virtual void EndUse();
	virtual void PlayAnimation();

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "FX")
	UNiagaraComponent* UseEffect;

protected:
	UPROPERTY(EditDefaultsOnly,Category="Tool|Paramter")
	float ConsumedDurability;
};
