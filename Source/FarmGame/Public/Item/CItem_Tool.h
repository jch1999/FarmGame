#pragma once

#include "CoreMinimal.h"
#include "Item/CItemBase.h"
#include "CItem_Tool.generated.h"

class UNiagaraComponent;
class ACPlayer;

UCLASS()
class FARMGAME_API ACItem_Tool : public ACItemBase
{
	GENERATED_BODY()

public:
	ACItem_Tool();

public:
	// From Interface
	virtual bool UseItem() override;;

	void SetOwnerPlayer(ACPlayer* InPlayer);
	const ACPlayer* GetOwnerPlayer();

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "FX")
	UNiagaraComponent* UseEffect;

private:
	ACPlayer* OwnerPlayer;
};
