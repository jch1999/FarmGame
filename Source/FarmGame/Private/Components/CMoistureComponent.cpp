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
	Super::AddValue(Amount);
	//UE_LOG(LogTemp, Warning, TEXT("AddMoisture : %f"), Amount);
	CheckState();
}

void UCMoistureComponent::ReduceMoisture(float Amount)
{
	Super::ReduceValue(Amount);

	CheckState();
}

void UCMoistureComponent::SetSafeRange(FVector2D InNewRange)
{
	Super::SetSafeRange(InNewRange);
	
	CheckState();
}


void UCMoistureComponent::SetDryState()
{
	//DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), "Moisture is under the safe rnage!", nullptr, FColor::Red, 0.8f);
	ChangeState(EMoistureState::Dry);
}

void UCMoistureComponent::SetEnoughState()
{
	ChangeState(EMoistureState::Enough);
}

void UCMoistureComponent::SetHumidState()
{
	//DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), "Moisture is over the safe rnage!", nullptr, FColor::Red, 0.8f);
	ChangeState(EMoistureState::Humid);
}

void UCMoistureComponent::AutoReduceValue()
{
	Super::AutoReduceValue();

	CheckState();
}

void UCMoistureComponent::CheckState()
{
	if (CurrentValue < SafeRange.X)
	{
		SetDryState();
		return;
	}
	else if (CurrentValue > SafeRange.Y)
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
