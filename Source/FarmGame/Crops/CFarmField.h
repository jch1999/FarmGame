#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CInterface_Interactable.h"
#include "CFarmField.generated.h"

class UCMoistureComponent;
class UCNutritionComponent;
class UCCultivationComponent;
class UStaticMeshComponent;
class ACBase_Crop;

UCLASS()
class FARMGAME_API ACFarmField : public AActor, public ICInterface_Interactable
{
	GENERATED_BODY()
	
public:	
	ACFarmField();

protected:
	virtual void BeginPlay() override;

public:	
	// Inherited via ICInterface_Interactable
	bool IsInteractable() override;
	void SetInteractable() override;
	void SetUnInteractable() override;
	EInteractObjectType GetType() override;
	void SetType(EInteractObjectType InNewType) override;
	void Interact() override;

	// Crop
	UFUNCTION(BlueprintCallable)
	void PlantCrop(ACBase_Crop* InCrop, FTransform& InTM);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCMoistureComponent* MoistureComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCNutritionComponent* NutritionComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCCultivationComponent* CultivationComp;

	UPROPERTY(EditAnywhere, Category="Crop")
	ACBase_Crop* Crop;
};
