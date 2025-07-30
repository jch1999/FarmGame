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
class UCameraComponent;
class ACBase_Crop;
class ACPlayer;
class ACPlayerController;

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
	virtual void SetDelayedInteractable(float DelayTime) override;

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	virtual void SetDelayedUninteractable(float DelayTime) override;

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
	ACBase_Crop* GetCrop() { return Crop; }
	UFUNCTION(BlueprintCallable)
	bool PlantCrop(TSubclassOf<ACBase_Crop> InCropClass, const FVector& RelativeOffset);
	void EraseCrop();

	void FarmFieldOn();
	void FarmFieldOff();

	// Component
	UFUNCTION(BlueprintPure)
	UCCultivationComponent* const GetCultivationComp() { return CultivationComp; }
	
	UFUNCTION(BlueprintPure)
	UCMoistureComponent* const GetMoistureComp() { return MoistureComp; }
	
	UFUNCTION(BlueprintPure)
	UCNutritionComponent* const GetNutritionComp() { return NutritionComp; }

private:
	void ShowFarmFieldWidget();

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

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Crop")
	ACBase_Crop* Crop;

	// Interact Interface
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractInterface")
	bool bInteractable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractInterface")
	EInteractObjectType InteractType;

private:
	UMaterialInstanceDynamic* FieldMaterial; 

	UPROPERTY()
	ACPlayerController* CachedPlayerController;

	FTimerHandle InteractTimer;
};
