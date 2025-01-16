#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGameModeBase.generated.h"

class UCCropWidget;
class UCFarmFieldWidget;

UCLASS()
class FARMGAME_API ACGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACGameModeBase();

protected:
	virtual void BeginPlay() override;
};
