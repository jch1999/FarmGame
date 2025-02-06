#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

UCLASS()
class FARMGAME_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()

	// Execute When Poseess
	virtual void OnPossess(APawn* aPawn) override;
	// Execute Whe Player State Changed
	virtual void OnRep_PlayerState() override;

public:
	virtual void SetupInputComponent();
	
};
