#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CGenericStateComponent.h"
#include "CMoistureComponent.generated.h"

UENUM(BlueprintType)
enum class EMoistureState :uint8
{
	Dry, Enough, Humid
};

// Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoisutreStateChanged, EMoistureState, InNewState);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMGAME_API UCMoistureComponent : public UCGenericStateComponent
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

	void SetDryState();
	void SetEnoughState();
	void SetHumidState();

	UFUNCTION(BlueprintPure)
	FORCEINLINE EMoistureState GetCurrentState() { return MoistureState; }

	void AddMoisture(float Amount);
	void ReduceMoisture(float Amount);
	virtual void SetSafeRange(FVector2D InNewRange) override;
	
private:
	virtual void AutoReduceValue() override;
	
	void CheckState();
	void ChangeState(EMoistureState state);

public:
	UPROPERTY(BlueprintAssignable)
	FMoisutreStateChanged OnMoistureStateChanged;
	
private:
	EMoistureState MoistureState;
};
