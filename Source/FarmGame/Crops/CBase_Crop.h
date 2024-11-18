#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBase_Crop.generated.h"

class UCMoistureComponent;
class UCNutritionComponent;

UCLASS()
class FARMGAME_API ACBase_Crop : public AActor
{
	GENERATED_BODY()
	
public:	
	ACBase_Crop();

protected:
	virtual void BeginPlay() override;

public:	

protected:
	UPROPERTY(VisibleAnywhere, Category="Component")
	UCMoistureComponent* MoistureComp;

};
