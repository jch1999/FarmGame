#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CInterface_Interactable.h"
#include "Components/CMoistureComponent.h"
#include "Components/CNutritionComponent.h"
#include "Engine/DataTable.h"
#include "CBase_Crop.generated.h"

class UCMoistureComponent;
class UCNutritionComponent;
class ACFarmField;
class UStaticMesh;

//UENUM(BlueprintType)
//enum class ECropGrowType :uint8
//{
//	VegetativeGrowth, ReproductiveGrowth, Maturity, Senescence, MAX
//};

USTRUCT(BlueprintType)
struct FCropData:public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CropName;
	
	// Grow
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GrowthLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetGrowthValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultGrowUpValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GrowDelay;

	// Moisture
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D SafeRange_M;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ConsumeMoisture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReduceDelay_M;

	// Nutirtion
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D SafeRange_N;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ConsumeNutrition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReduceDelay_N;

	// Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
public:	
	// Inherited via ICInterface_Interactable
	bool IsInteractable() override;

	void SetInteractable() override;

	void SetUnInteractable() override;

	FORCEINLINE EInteractObjectType GetType() override { return Type; };

	void SetType(EInteractObjectType InNewType) override;

	void Interact() override;

	// CropData
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FCropData GetCurrentCropData() { return Datas[NowGrowLevel]; }
	
	// Grow
	UFUNCTION(BlueprintCallable)
	void SetAutoGrowTimer(float InFirstDelay, bool InbLoop = false, float InLoopDelay = 0.0f);

	UFUNCTION(BlueprintCallable)
	void GrowUp();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsHarvestable() { return NowGrowLevel == MaxGrowLevel; }

	UFUNCTION(BlueprintCallable)
	void SetOwnerField(ACFarmField* InOwnerField);

private:
	void SetCropData();

	void AutoGrow();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category="Component")
	UCMoistureComponent* MoistureComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCNutritionComponent* NutritionComp;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	FName CropName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable")
	int32 NowGrowLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DataTable")
	int32 MaxGrowLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DataTable")
	float NowGrowValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="DataTable")
	UDataTable* CropData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DataTable")
	TArray<FCropData> Datas;

private:
	FTimerHandle AutoGrowTimer;
	ACFarmField* OwnerField;
	TArray<UStaticMesh*> CropMeshes;
};
