#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CItemInterface.h"
#include "CBase_CropSeed.generated.h"

class ACBase_Crop;

UCLASS()
class FARMGAME_API ACBase_CropSeed : public AActor, public ICItemInterface
{
	GENERATED_BODY()
	
public:	
	ACBase_CropSeed();

public:
	// Inherited via ICItemInterface
	UFUNCTION(BlueprintCallable)
	bool IsUsable() const override;

	UFUNCTION(BlueprintCallable)
	void SetUsable() override;

	UFUNCTION(BlueprintCallable)
	void SetUnsable() override;

	UFUNCTION(BlueprintCallable)
	bool UseItem() override;

	UFUNCTION(BlueprintCallable)
	void AddAvailableCount(int32 InAmount) override;

	UFUNCTION(BlueprintCallable)
	void ReduceAvailableCount(int32 InAmount) override;

	UFUNCTION(BlueprintCallable)
	int32 GetAvailableCount() const override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Crop")
	TSubclassOf<ACBase_Crop> CropClass;

	UPROPERTY(EditDefaultsOnly, Category = "Crop")
	float NeedCultivation;
};
