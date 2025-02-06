#include "CGameModeBase.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "UI/CCropWidget.h"
#include "UI/CFarmFieldWidget.h"

ACGameModeBase::ACGameModeBase()
{
	CHelpers::GetClass(&DefaultPawnClass, "/Game/Player/BP_CPlayer");
	CHelpers::GetClass(&PlayerControllerClass, "/Game/Controller/BP_CPlayerController");
	//CHelpers::GetClass(&)
}

void ACGameModeBase::BeginPlay()
{
}