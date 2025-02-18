// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/CItemBase.h"
#include "MyCItem_Tool.generated.h"

/**
 * 
 */
UCLASS()
class FARMGAME_API AMyCItem_Tool : public ACItemBase
{
	GENERATED_BODY()

	// From Interface
	virtual bool UseItem() override;;

	virtual FName GetInteractName() override;
};
