#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CInterface_Interactable.h"
#include "Components/CMoistureComponent.h"
#include "Components/CNutritionComponent.h"
#include "CBase_Crop.generated.h"

class UCMoistureComponent;
class UCNutritionComponent;

UENUM(BlueprintType)
enum class ECropGrowType :uint8
{
	VegetativeGrowth, ReproductiveGrowth, Maturity, Senescence, MAX
};

USTRUCT(BlueprintType)
struct FCropData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName CropName;

	UPROPERTY(EditAnywhere)
	int32 GrowthLevel;

	// Grow Time
	float TargetGrowthValue;

	// Moisture
	FVector2D SafeRange_M;

	UPROPERTY(EditAnywhere)
	float ConsumeMoisture;

	// Nutirtion
	FVector2D SafeRange_N;

	UPROPERTY(EditAnywhere)
	float ConsumeNutrition;

	// Mesh
	UPROPERTY(EditAnywhere)
	FString MeshRef;
};

UCLASS()
class FARMGAME_API ACBase_Crop : public AActor, public ICInterface_Interactable
{
	GENERATED_BODY()
	
public:	
	ACBase_Crop();

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

	FCropData GetCurrentCropData();
	void Grow();
	void SetGrowType();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category="Component")
	UCMoistureComponent* MoistureComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCNutritionComponent* NutritionComp;

protected:
	FCropData Datas[(int32)ECropGrowType::MAX];
	ECropGrowType GrowType;
};
