
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CInterface_Interactable.generated.h"

UENUM(BlueprintType)
enum class EInteractObjectType :uint8
{
	None, Player, Crop, Marchandise, FarmField, Item, Max
};

UINTERFACE(MinimalAPI)
class UCInterface_Interactable : public UInterface
{
	GENERATED_BODY()
};

class FARMGAME_API ICInterface_Interactable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "InteractInterface")
	virtual bool IsInteractable() = 0;

	UFUNCTION(BlueprintCallable, Category = "InteractInterface")
	virtual void SetInteractable() = 0;

	UFUNCTION(BlueprintCallable, Category = "InteractInterface")
	virtual void SetUnInteractable() = 0;

	UFUNCTION(BlueprintCallable, Category = "InteractInterface")
	virtual EInteractObjectType GetType() = 0;

	UFUNCTION(BlueprintCallable, Category = "InteractInterface")
	virtual void SetType(EInteractObjectType InNewType) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InteractInterface")
	void Interact();

protected:
	bool bInteractable;
	EInteractObjectType InteractType;
};
