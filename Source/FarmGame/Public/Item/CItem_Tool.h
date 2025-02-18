// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/CItemBase.h"
#include "CItem_Tool.generated.h"

/**
 * 
 */
UCLASS()
class FARMGAME_API ACItem_Tool : public ACItemBase
{
	GENERATED_BODY()

public:
	ACItem_Tool();

	// From Interface
	virtual bool UseItem() override;;
};
