#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

UCLASS()
class FARMGAME_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent();
	
};
