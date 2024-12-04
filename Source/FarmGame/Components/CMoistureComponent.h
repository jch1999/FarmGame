#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMoistureComponent.generated.h"

UENUM(BlueprintType)
enum class EMoistureState :uint8
{
	Dry, Enough, Humid
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoisutreStateChanged, EMoistureState, InNewState);

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
	FORCEINLINE FVector2D GetSafeRange() { return MoistureSafeRange; }
	FORCEINLINE float GetMoistureValue() { return NowMoisture; }

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

protected:
	UPROPERTY(EditAnywhere, Category="Moisture")
	float NowMoisture;

	UPROPERTY(EditAnywhere, Category = "Moisture")
	float AutoReduceAmount;

	UPROPERTY(EditAnywhere, Category = "Moisture")
	FVector2D MoistureSafeRange;

private:
	FTimerHandle MoistureReduceTimer;
	EMoistureState MoistureState;
};
