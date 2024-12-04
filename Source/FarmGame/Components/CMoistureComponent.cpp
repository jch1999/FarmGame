#include "Components/CMoistureComponent.h"
#include "Global.h"

UCMoistureComponent::UCMoistureComponent()
{
}


void UCMoistureComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCMoistureComponent::AddMoisture(float Amount)
{
	NowMoisture += Amount;

	CheckState();
}

void UCMoistureComponent::ReduceMoisture(float Amount)
{
	NowMoisture -= Amount;
	NowMoisture = FMath::Clamp(NowMoisture, 0, NowMoisture > MoistureSafeRange.Y ? NowMoisture : MoistureSafeRange.Y);
	
	CheckState();
}

void UCMoistureComponent::SetSafeRange(FVector2D NewRange)
{
	MoistureSafeRange = NewRange;
}

void UCMoistureComponent::SetAutoReduceAmount(float InReduceAmount)
{
	AutoReduceAmount = InReduceAmount;
}

void UCMoistureComponent::SetDryState()
{
	DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), "Moisture is under the safe rnage!", nullptr, FColor::Red, 0.8f);
	ChangeState(EMoistureState::Dry);
}

void UCMoistureComponent::SetEnoughState()
{
	ChangeState(EMoistureState::Enough);
}

void UCMoistureComponent::SetHumidState()
{
	DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), "Moisture is over the safe rnage!", nullptr, FColor::Red, 0.8f);
	ChangeState(EMoistureState::Humid);
}

void UCMoistureComponent::SetAutoReduceTimer(float InFirstDelay, bool InbLoop, float InLoopDelay)
{
	GetWorld()->GetTimerManager().ClearTimer(MoistureReduceTimer);

	GetWorld()->GetTimerManager().SetTimer(MoistureReduceTimer, this, &UCMoistureComponent::AutoReduceMoisture, InLoopDelay, InbLoop, InFirstDelay);
}

void UCMoistureComponent::AutoReduceMoisture()
{
	ReduceMoisture(AutoReduceAmount);
}

void UCMoistureComponent::CheckState()
{
	if (NowMoisture < MoistureSafeRange.X)
	{
		SetDryState();
		return;
	}
	else if (NowMoisture > MoistureSafeRange.Y)
	{
		SetHumidState();
		return;
	}

	SetEnoughState();
}

void UCMoistureComponent::ChangeState(EMoistureState state)
{
	CheckTrue(MoistureState == state);

	MoistureState = state;

	OnMoistureStateChanged.Broadcast(state);
}
