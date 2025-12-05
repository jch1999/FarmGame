#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Test.generated.h"

UINTERFACE(MinimalAPI)
class UTest : public UInterface
{
	GENERATED_BODY()
};

class FARMGAME_API ITest
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="MyInterface")
	bool Test();
};
