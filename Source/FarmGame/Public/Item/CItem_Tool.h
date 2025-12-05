#pragma once

#include "CoreMinimal.h"
#include "Item/CItemBase.h"
#include "Interfaces/UsableItem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "CItem_Tool.generated.h"

class ACPlayer;

UCLASS()
class FARMGAME_API ACItem_Tool : public ACItemBase, public IUsableItem
{
	GENERATED_BODY()

public:
	ACItem_Tool();

public:
	// From Interface
	virtual bool UseItem_Implementation();
	virtual void EndUse_Implementation();
	virtual bool PlayAnimation_Implementation();
	virtual bool IsUsable_Implementation() const override;
	virtual void SetUsable_Implementation() override;
	virtual void SetUnusable_Implementation() override;

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "FX")
	UNiagaraComponent* UseEffect;

protected:
	UPROPERTY(EditDefaultsOnly,Category="Tool|Paramter")
	float ConsumedDurability;

	// 사용 가능 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Usable")
	bool bUsable;
};
