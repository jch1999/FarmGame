#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CGenericStateComponent.h"
#include "CCultivationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMGAME_API UCCultivationComponent : public UCGenericStateComponent
{
	GENERATED_BODY()

public:	
	UCCultivationComponent();

protected:
	virtual void BeginPlay() override;

public:
	void AddCultivation(float InAmount);
	void ReduceCultivation(float InAmount);

protected:
	UPROPERTY(EditAnywhere, Category = "Cultivation")
	FVector2D CultivationRange;

};
