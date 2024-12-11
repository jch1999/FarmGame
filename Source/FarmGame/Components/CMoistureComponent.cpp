#include "Components/CMoistureComponent.h"
#include "Global.h"

UCMoistureComponent::UCMoistureComponent()
{
	MaxMoisture = 100.0f;
}


void UCMoistureComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCMoistureComponent::AddMoisture(float Amount)
{
	CurrentMoisture += Amount;
	CurrentMoisture = FMath::Clamp(CurrentMoisture, 0, CurrentMoisture > MoistureSafeRange.Y ? CurrentMoisture : MoistureSafeRange.Y);

	OnMoistureChanged.Broadcast();

	CheckState();
}

void UCMoistureComponent::ReduceMoisture(float Amount)
{
	CurrentMoisture -= Amount;
	CurrentMoisture = FMath::Clamp(CurrentMoisture, 0, CurrentMoisture > MoistureSafeRange.Y ? CurrentMoisture : MoistureSafeRange.Y);
	
	OnMoistureChanged.Broadcast();

	CheckState();
}

void UCMoistureComponent::SetSafeRange(FVector2D NewRange)
{
	MoistureSafeRange = NewRange;

	OnMoistureChanged.Broadcast();
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
	if (CurrentMoisture < MoistureSafeRange.X)
	{
		SetDryState();
		return;
	}
	else if (CurrentMoisture > MoistureSafeRange.Y)
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
