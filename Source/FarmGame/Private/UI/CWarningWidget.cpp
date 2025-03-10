#include "UI/CWarningWidget.h"
#include "Components/TextBlock.h"
#include "UI/CTitleBarWidget.h"
#include "Components/Button.h"
#include "CGameInstance.h"

bool UCWarningWidget::Initialize()
{
    bool bSuccess = Super::Initialize();
    if (!bSuccess) return false;

    if (TitleBarWidget)
    {
        TitleBarWidget->SetParentWidget(this);
        TitleBarWidget->SetTitle("Warning!", FLinearColor::Red);

        if (UGameInstance* GI = GetGameInstance())
        {
            if (UCGameInstance* MyGI = Cast<UCGameInstance>(GI))
            {
                TitleBarWidget->CloseButton->OnClicked.AddDynamic(MyGI, &UCGameInstance::HideWarningWidget);
            }
        }
    }

    return true;
}

void UCWarningWidget::SetWarningText(FString InWarningMsg)
{
	WarningText->SetText(FText::FromString(InWarningMsg));
}
