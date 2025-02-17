#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CInventoryComponent.generated.h"

class UUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMGAME_API UCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	// Widget
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> QuickSlotWidgetClass;
		
};
