#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CInterface_Interactable.h"
#include "CFarmField.generated.h"

class UCMoistureComponent;
class UCNutritionComponent;
class UCCultivationComponent;
class UWidgetComponent;
class UStaticMeshComponent;
class UBoxComponent;
class UCFarmFieldWidget;
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
	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	bool IsInteractable() override { return bInteractable; }

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	void SetInteractable() override;

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	void SetUnInteractable() override;

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	void SetType(EInteractObjectType InNewType) override;

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	EInteractObjectType GetType() override { return InteractType; }

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	virtual FName GetInteractName() override { return "FarmField"; }

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	virtual void Interact(AActor* OtherActor) override;

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	bool OnHovered() override;
	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	bool OnUnhovered() override;


	// Crop
	UFUNCTION(BlueprintCallable)
	bool PlantCrop(TSubclassOf<ACBase_Crop> InCropClass, const FTransform& InTM);

	// Component
	UFUNCTION(BlueprintPure)
	UCCultivationComponent* const GetCultivationComp() { return CultivationComp; }
	
	UFUNCTION(BlueprintPure)
	UCMoistureComponent* const GetMoistureComp() { return MoistureComp; }
	
	UFUNCTION(BlueprintPure)
	UCNutritionComponent* const GetNutritionComp() { return NutritionComp; }

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCMoistureComponent* MoistureComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCNutritionComponent* NutritionComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCCultivationComponent* CultivationComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UWidgetComponent* InfoWidgetComp;

	// Widget
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TSubclassOf<UCFarmFieldWidget> WidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
	UCFarmFieldWidget* InfoWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Crop")
	ACBase_Crop* Crop;

private:
	UMaterialInstanceDynamic* FieldMaterial;
};
