#include "CHUD.h"
#include "UI/CHUDWidget.h"

void ACHUD::CreateHUD(APlayerController* PC)
{
	if (UIHudWidget != nullptr) return;

	if (UIHudWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing UIHUDWidgetClass."), *FString(__FUNCTION__));
		return;
	}

	UIHudWidget = CreateWidget<UCHUDWidget>(PC, UIHudWidgetClass);
	UIHudWidget->AddToViewport();
	UE_LOG(LogTemp, Warning, TEXT("UIHUDClass Created!"));
}

UCHUDWidget* ACHUD::GetHUD()
{
	return UIHudWidget;
}
