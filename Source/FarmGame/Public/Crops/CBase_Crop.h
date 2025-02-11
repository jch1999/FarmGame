#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CInterface_Interactable.h"
#include "Engine/DataTable.h"
#include "CBase_Crop.generated.h"

class UCMoistureComponent;
class UCNutritionComponent;
class UCHealthComponent;
class ACFarmField;
class UStaticMesh;

UENUM(BlueprintType)
enum class ECropGrowStage :uint8
{
	VegetativeGrowth, ReproductiveGrowth, Maturity, Senescence, MAX
};

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

	// Moisture
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D SafeRange_Moisture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Max_Moisture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ConsumeMoisture;

	// Nutirtion
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D SafeRange_Nutrition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Max_Nutrition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ConsumeNutrition;

	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Max_Health;

	// Price Range
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D PriceRange;

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

	//virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:	
	// Inherited via ICInterface_Interactable
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
	virtual FName GetInteractName() override { return "Crop"; }

	virtual void Interact(AActor* OtherActor) override;

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

	// Get Components
	UFUNCTION(BlueprintPure)
	FORCEINLINE UCHealthComponent* const GetHealthComp() { return HealthComp; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE UCNutritionComponent* const GetNutritionComp() { return NutritionComp; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE UCMoistureComponent* const GetMoistureComp() { return MoistureComp; }

private:
	void AutoGrow();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category="Component")
	UCMoistureComponent* MoistureComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCNutritionComponent* NutritionComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCHealthComponent* HealthComp;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	FName CropName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable")
	int32 NowGrowLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DataTable")
	int32 MaxGrowLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DataTable")
	float NowGrowValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable")
	float UpdateTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="DataTable")
	UDataTable* CropData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DataTable")
	TArray<FCropData> Datas;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Meshes")
	TArray<UStaticMesh*> CropMeshes;

private:
	FTimerHandle AutoGrowTimer;
	ACFarmField* OwnerField;

};
