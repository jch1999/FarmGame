#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CInterface_Interactable.h"
#include "Interfaces/CItemInterface.h"
#include "Engine/DataTable.h"
#include "CBase_Crop.generated.h"

class UCMoistureComponent;
class UCNutritionComponent;
class UCHealthComponent;
class ACFarmField;
class UStaticMesh;
class ACItem_Crop;

UENUM(BlueprintType)
enum class ECropGrowStage :uint8
{
	VegetativeGrowth, ReproductiveGrowth, Maturity, Senescence, MAX
};

USTRUCT(BlueprintType)
struct FCropGrowthData: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CropName;

	// Growth
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

	// Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MeshRef;
};

USTRUCT(BlueprintType)
struct FCropData:public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CropName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EItemID> IDForQuality;

	// Effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GrowUpPraticleEffectRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GrowUpSoundEffectRef;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TArray<FString> HarvetIDForQuality;
};

class UBoxComponent;

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
	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	bool IsInteractable() override { return bInteractable; }

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	void SetInteractable() override;

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	void SetUnInteractable() override;

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	void SetType(EInteractObjectType InNewType) override;

	UFUNCTION(BlueprintPure, Category = "InteracteInterface")
	EInteractObjectType GetType() override { return InteractType; }

	UFUNCTION(BlueprintPure, Category = "InteracteInterface")
	virtual FName GetInteractName() override { return "Crop"; }

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	virtual void Interact(AActor* OtherActor) override;

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	bool OnHovered() override;
	
	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	bool OnUnhovered() override;

	// Grow
	UFUNCTION(BlueprintCallable)
	void SetAutoGrowTimer(float InFirstDelay, bool InbLoop = false, float InLoopDelay = 0.0f);

	UFUNCTION(BlueprintCallable)
	void GrowUp();

	UFUNCTION(BlueprintCallable)
	void PlayGrowthEffects();

	UFUNCTION(BlueprintPure)
	bool IsDead() { return GetHealthComp()->GetCurrentHealth() < 0.0f; }

	// Harvest
	UFUNCTION(BlueprintPure)
	bool IsHarvestable();

	void DoHarvest();
	
	EQualityType GetCropQuality() { return CropQuality; }
	void SetCropQuality(EQualityType InType);

	UFUNCTION()
	void ChangeQualityByHealth(float CureentHealth, float PrevHealth, float MaxHealth);

	// Get Components
	UFUNCTION(BlueprintPure)
	FORCEINLINE UCHealthComponent* const GetHealthComp() { return HealthComp; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE UCNutritionComponent* const GetNutritionComp() { return NutritionComp; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE UCMoistureComponent* const GetMoistureComp() { return MoistureComp; }

private:
	void AutoGrow();
	void SetCropDatas();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category="Component")
	UCMoistureComponent* MoistureComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCNutritionComponent* NutritionComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCHealthComponent* HealthComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UBoxComponent* BoxComp;

	// Effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* GrowthParticleEffect;  // ���� ��ƼŬ ȿ��

	// ���� ȿ�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* GrowthSoundEffect;  // ���� ���� ȿ��

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Crop|Defualt")
	FName CropName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crop|Defualt")
	int32 CurrentGrowLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Crop|Defualt")
	float CurrentGrowValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crop|Time")
	float UpdateTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crop|Harvest")
	TArray<FTransform> SpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crop|Harvest")
	EQualityType CropQuality;
	
	// Interact Interface
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractInterface")
	bool bInteractable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractInterface")
	EInteractObjectType InteractType;

private:
	FTimerHandle AutoGrowTimer;
	ACFarmField* OwnerField;

};
