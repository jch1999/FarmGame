#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CItemInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FARMGAME_API ICItemInterface
{
	GENERATED_BODY()
public:
	virtual bool IsUsable() const = 0;
	virtual void SetUsable() = 0;
	virtual void SetUnsable() = 0;
	virtual bool UseItem() = 0;

	virtual void AddAvailableCount(int32 InAmount = 1) = 0;
	virtual void ReduceAvailableCount(int32 InAmount = 1) = 0;
	virtual int32 GetAvailableCount() const = 0;

protected:
	bool bUsable;
	int32 AvailableCount;
};
