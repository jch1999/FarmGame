#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CInteractComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMGAME_API UCInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCInteractComponent();

protected:
	virtual void BeginPlay() override;

public:	
		
};
