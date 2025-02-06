#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMoistureComponent.generated.h"



UENUM(BlueprintType)
enum class EMoistureState :uint8
{
	Dry, Enough, Humid
};

// Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoisutreStateChanged, EMoistureState, InNewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FMoistureChanged, float, OldValue, float, NewValue, float, MaxValue);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMGAME_API UCMoistureComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCMoistureComponent();

protected:
	virtual void BeginPlay() override;

public:	
	FORCEINLINE bool IsEnough() { return MoistureState==EMoistureState::Enough; }
	FORCEINLINE bool IsDry() { return MoistureState==EMoistureState::Dry; }
	FORCEINLINE bool ISHumid() { return MoistureState == EMoistureState::Humid;}

	UFUNCTION(BlueprintPure)
	FORCEINLINE FVector2D GetSafeRange() { return MoistureSafeRange; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMaxMoisture() { return MaxMoisture; }
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentMoisture() { return CurrentMoisture; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentRate() { return CurrentMoisture /MaxMoisture; }

	void AddMoisture(float Amount);
	void ReduceMoisture(float Amount);
	void SetSafeRange(FVector2D NewRange);
	void SetAutoReduceAmount(float InReduceAmount);

	void SetDryState();
	void SetEnoughState();
	void SetHumidState();

	UFUNCTION(BlueprintCallable)
	void SetAutoReduceTimer(float InFirstDelay, bool InbLoop = false, float InLoopDelay = 0.0f);

private:
	void AutoReduceMoisture();
	
	void CheckState();
	void ChangeState(EMoistureState state);

public:
	UPROPERTY(BlueprintAssignable)
	FMoisutreStateChanged OnMoistureStateChanged;

	UPROPERTY(BlueprintAssignable)
	FMoistureChanged OnMoistureChanged;
	
protected:
	UPROPERTY(EditAnywhere, Category="Moisture")
	float CurrentMoisture;

	UPROPERTY(EditDefaultsOnly, Category = "Moisture")
	float MaxMoisture;

	UPROPERTY(EditAnywhere, Category = "Moisture")
	float AutoReduceAmount;

	UPROPERTY(EditAnywhere, Category = "Moisture")
	FVector2D MoistureSafeRange;

private:
	FTimerHandle MoistureReduceTimer;
	EMoistureState MoistureState;
};
