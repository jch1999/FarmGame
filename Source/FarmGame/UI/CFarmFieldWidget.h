#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CFarmFieldWidget.generated.h"

class ACFarmField;

UCLASS()
class FARMGAME_API UCFarmFieldWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetFarmField(const ACFarmField* const InFarmField);

	UFUNCTION(BlueprintImplementableEvent)
	void ResetFarmField();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE  bool IsAvailable() { return FarmField == nullptr; }

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="FarmField")
	ACFarmField* FarmField;
	
};
