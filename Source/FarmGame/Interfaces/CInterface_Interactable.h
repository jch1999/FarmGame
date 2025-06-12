
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CInterface_Interactable.generated.h"

UENUM(BlueprintType)
enum class EInteractObjectType :uint8
{
	None, Player, Crop, NPC, FarmField, Item, Tool, Max
};

USTRUCT(BlueprintType)
struct FInteractAssetData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInteractObjectType InteractType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString InteractIconTextureRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString InteractSFXRef;
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
	virtual bool IsInteractable() = 0;
	virtual void SetInteractable() = 0;
	virtual void SetUnInteractable() = 0;
	virtual void SetDelayedInteractable(float DelayTime) = 0;
	virtual void SetDelayedUninteractable(float DelayTime) = 0;
	virtual EInteractObjectType GetType() = 0;
	virtual FName GetInteractName() = 0;
	virtual void SetType(EInteractObjectType InNewType) = 0;
	virtual void Interact(AActor* OtherActor) = 0;
	virtual bool OnHovered() = 0;
	virtual bool OnUnhovered() = 0;
};
