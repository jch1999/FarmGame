#include "UI/CWarningWidget.h"
#include "Components/TextBlock.h"
#include "UI/CTitleBarWidget.h"
#include "Components/Button.h"
#include "Controller/CPlayerController.h"

bool UCWarningWidget::Initialize()
{
    bool bSuccess = Super::Initialize();
    if (!bSuccess) return false;

    if (TitleBarWidget)
    {
        TitleBarWidget->SetParentWidget(this);
        TitleBarWidget->SetTitle("Warning!", FLinearColor::Red);

        if (APlayerController* PC = GetOwningPlayer<APlayerController>())
        {
            if (ACPlayerController* MyPC = Cast<ACPlayerController>(PC))
            {
                TitleBarWidget->CloseButton->OnClicked.AddDynamic(MyPC, &ACPlayerController::HideWarningWidget);
            }
        }
    }

    return true;
}

void UCWarningWidget::SetWarningText(FString InWarningMsg)
{
	WarningText->SetText(FText::FromString(InWarningMsg));
}
