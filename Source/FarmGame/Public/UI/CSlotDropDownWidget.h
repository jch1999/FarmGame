#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSlotDropDownWidget.generated.h"

class ICItemInterface;

UCLASS()
class FARMGAME_API UCSlotDropDownWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetItem(EItemID InItemID);

public:

};
