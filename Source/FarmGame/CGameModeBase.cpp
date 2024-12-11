#include "CGameModeBase.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "UI/CCropWidget.h"

ACGameModeBase::ACGameModeBase()
{
	CHelpers::GetClass(&DefaultPawnClass, "/Game/Player/BP_CPlayer");
	CHelpers::GetClass(&PlayerControllerClass, "/Game/Controller/BP_CPlayerController");
	CHelpers::GetClass(&CropWidgetClass, "/Game/UI/WB_CropWidget.WB_CropWidget");
}

void ACGameModeBase::BeginPlay()
{
	if (CropWidgetClass)
	{
		CropWidget = CreateWidget<UCCropWidget>(GetWorld(), CropWidgetClass);
	}

	if (FarmFieldWidgetClass)
	{
		//FarmFieldWidget = CreateWidget(this, FarmFieldWidgetClass);
	}
}
