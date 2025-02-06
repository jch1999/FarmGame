#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CTestInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCTestInterface : public UInterface
{
	GENERATED_BODY()
};

class FARMGAME_API ICTestInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="MyInterface")
	bool Test();
};
