#include "CGameModeBase.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "UI/CCropWidget.h"
#include "UI/CFarmFieldWidget.h"

ACGameModeBase::ACGameModeBase()
{
	CHelpers::GetClass(&DefaultPawnClass, "/Game/Player/BP_CPlayer");
	CHelpers::GetClass(&PlayerControllerClass, "/Game/Controller/BP_CPlayerController");
	CHelpers::GetClass(&CropWidgetClass, "/Game/UI/WB_CropWidget");
	CHelpers::GetClass(&FarmFieldWidgetClass, "/Game/UI/WB_FarmFieldWidget");
}

void ACGameModeBase::BeginPlay()
{
}

UCCropWidget* ACGameModeBase::GetCropWidget()
{
	CheckNullResult(CropWidgetClass, nullptr);

	if (!CropWidget)
	{
		CropWidget = CreateWidget<UCCropWidget>(GetWorld(), CropWidgetClass);
	}

	return CropWidget;
}

UCFarmFieldWidget* ACGameModeBase::GetFarmFieldWidget()
{
	CheckNullResult(FarmFieldWidgetClass, nullptr);

	if (!FarmFieldWidget)
	{
		FarmFieldWidget = CreateWidget<UCFarmFieldWidget>(GetWorld(), FarmFieldWidgetClass);
	}

	return FarmFieldWidget;
}

void ACGameModeBase::AddFarmVale(float InAddAmount)
{
}

void ACGameModeBase::ReduceFarmVale(float InReduceAmount)
{
}
