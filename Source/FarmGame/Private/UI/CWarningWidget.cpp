#include "UI/CWarningWidget.h"
#include "Components/TextBlock.h"
#include "UI/CTitleBarWidget.h"
#include "Controller/CPlayerController.h"
#include "Components/Button.h"

bool UCWarningWidget::Initialize()
{
    bool bSuccess = Super::Initialize();
    if (!bSuccess) return false;

    if (TitleBarWidget)
    {
        TitleBarWidget->SetParentWidget(this);

        if (APlayerController* Controller = GetOwningPlayer())
        {
            if (ACPlayerController* MyController = Cast<ACPlayerController>(Controller))
            {
                TitleBarWidget->CloseButton->OnClicked.AddDynamic(this, &UCWarningWidget::CloseWarning);
            }
        }
    }
}

void UCWarningWidget::SetWarningText(FString InWarningMsg)
{
	WarningText->SetText(FText::FromString(InWarningMsg));
}

void UCWarningWidget::CloseWarning()
{
    SetVisibility(ESlateVisibility::Hidden);
}
