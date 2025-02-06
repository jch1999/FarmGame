#include "Controller/CPlayerController.h"
#include "CHUD.h"

void ACPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if (!IsLocalController())
	{
		UE_LOG(LogTemp, Warning, TEXT("Is not local controller"));
		return;
	}

	AHUD* Hud = GetHUD();
	if (IsValid(Hud))
	{
		ACHUD* MyHud = Cast<ACHUD>(Hud);
		if (IsValid(MyHud))
		{
			MyHud->CreateHUD();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HUD is not valid in OnPossess"));
		}
	}
}

void ACPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	if (!IsLocalController())
	{
		UE_LOG(LogTemp, Warning, TEXT("Is not local controller"));
		return;
	}

	AHUD* Hud = GetHUD();
	if (IsValid(Hud))
	{
		ACHUD* MyHud = Cast<ACHUD>(Hud);
		if (IsValid(MyHud))
		{
			MyHud->CreateHUD();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HUD is not valid in OnPossess"));
		}
	}
}

void ACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}