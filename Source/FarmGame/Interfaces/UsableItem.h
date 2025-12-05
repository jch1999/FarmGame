#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UsableItem.generated.h"

UINTERFACE(MinimalAPI)
class UUsableItem : public UInterface
{
	GENERATED_BODY()
};

class FARMGAME_API IUsableItem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item|Usable")
	bool UseItem();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item|Usable")
	void EndUse();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item|Usable")
	bool PlayAnimation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item|Usable")
	bool IsUsable() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item|Usable")
	void SetUsable();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item|Usable")
	void SetUnusable();
};
