#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWarningWidget.generated.h"

class UCTitleBarWidget;
class UTextBlock;

UCLASS()
class FARMGAME_API UCWarningWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize() override;

public:
	void SetWarningText(FString InWarningMsg);

public:
	UPROPERTY(meta = (BindWidget))
	UCTitleBarWidget* TitleBarWidget;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WarningText;
};
