#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CCropWidget.generated.h"

class ACBase_Crop;

UCLASS()
class FARMGAME_API UCCropWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetCrop(const ACBase_Crop* const InCrop);

	UFUNCTION(BlueprintImplementableEvent)
	void ResetCrop();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE  bool IsAvailable() { return Crop == nullptr; }

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Crop")
	ACBase_Crop* Crop;
	
};
