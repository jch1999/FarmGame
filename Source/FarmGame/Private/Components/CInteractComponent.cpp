#include "Components/CInteractComponent.h"
#include "GameFramework/Character.h"
#include "Controller/CPlayerController.h"
#include "Global.h"
#include "CHUD.h"
#include "UI/CHUDWidget.h"
#include "Characters/CPlayer.h"
#include "Camera/CameraComponent.h"

UCInteractComponent::UCInteractComponent()
{
	DetectRange = 150.0f;
	RemoveRange = 300.0f;
}

void UCInteractComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ACPlayer>(GetOwner());
	
	SetDetectInterval(0.2f);
}

void UCInteractComponent::DoInteract(AActor* OtherActor)
{
	if (OtherActor == nullptr)
	{
		if (InteractableObjects.Num() == 0) return;

		ACPlayerController* PC = Cast<ACPlayerController>(OwnerCharacter->GetController());
		if (PC)
		{
			ACHUD* MyHud = Cast<ACHUD>(PC->GetHUD());

			if (MyHud)
			{
				UCHUDWidget* HUDWidget=MyHud->GetHUD();
				if (HUDWidget)
				{
					AActor* InteractTarget = HUDWidget->GetInteractTarget();
					CheckNull(InteractTarget);

					// HUDWidget->RemoveInteractRow(InteractTarget);
					ICInterface_Interactable* InteractActor = Cast<ICInterface_Interactable>(InteractTarget);
					InteractActor->Interact(OwnerCharacter);
					RemoveInteractableObject(InteractTarget);
				}
			}
		}
	}
}


void UCInteractComponent::AddInteractableObject(AActor* InActor)
{
	if (InActor && InActor->Implements<UCInterface_Interactable>())
	{
		int32 BeforeLen = InteractableObjects.Num();
		InteractableObjects.AddUnique(InActor);

		if (BeforeLen != InteractableObjects.Num())
		{
			ACPlayerController* PC = Cast<ACPlayerController>(OwnerCharacter->GetController());
			if (PC)
			{
				ACHUD* MyHud = Cast<ACHUD>(PC->GetHUD());

				if (MyHud)
				{
					UCHUDWidget* HUDWidget = MyHud->GetHUD();
					if (HUDWidget)
					{
						HUDWidget->AddInteractRow(InActor);
						UE_LOG(LogTemp, Warning, TEXT("Added Interactable Object: %s"), *(InActor->GetActorLabel()));
					}
				}
			}
		}
	}
}

void UCInteractComponent::RemoveInteractableObject(AActor* InActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Before Remove: InteractableObjects.Num() = %d"), InteractableObjects.Num());

	if (InActor && InActor->Implements<UCInterface_Interactable>())
	{
		int32 BeforeLen = InteractableObjects.Num();
		InteractableObjects.RemoveAll([&](AActor* Actor) {return Actor == InActor; });
		int32 AfterLen = InteractableObjects.Num();;

		if (BeforeLen != AfterLen)
		{
			ACPlayerController* PC = Cast<ACPlayerController>(OwnerCharacter->GetController());
			if (PC)
			{
				ACHUD* MyHud = Cast<ACHUD>(PC->GetHUD());

				if (MyHud)
				{
					UCHUDWidget* HUDWidget = MyHud->GetHUD();
					if (HUDWidget)
					{
						HUDWidget->RemoveInteractRow(InActor);
						UE_LOG(LogTemp, Warning, TEXT("Removed Interactable Object: %s"), *InActor->GetActorLabel());
						UE_LOG(LogTemp, Warning, TEXT("After Remove: InteractableObjects.Num() = %d, InteractIndex = %d"), InteractableObjects.Num(), HUDWidget->GetInteractIndex());
					}
				}
			}
		}
	}
}

void UCInteractComponent::Scroll(float InputValue)
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastScrollTime < ScrollCooldown)
	{
		return;
	}
	LastScrollTime = CurrentTime;

	if (InteractableObjects.Num() > 0)
	{
		ACPlayerController* PC = Cast<ACPlayerController>(OwnerCharacter->GetController());
		if (PC)
		{
			ACHUD* MyHud = Cast<ACHUD>(PC->GetHUD());

			if (MyHud)
			{
				UCHUDWidget* HUDWidget = MyHud->GetHUD();
				if (HUDWidget)
				{
					if (InputValue > 0)
					{
						HUDWidget->UpInteractIndex();
					}
					else if (InputValue < 0)
					{
						HUDWidget->DownInteractIndex();
					}
				}
				UE_LOG(LogTemp, Warning, TEXT("Scroll After: InteractIndex = %d"), HUDWidget->GetInteractIndex());
			}
		}
	}
}

void UCInteractComponent::DetectInteractableObjects()
{
	if (!OwnerCharacter) return;
	if (!OwnerCharacter->IsInteractable()) return;

	// Range Detect
	// Find Objects away from detect range
	TArray<AActor*> ToRemoveActors;
	for (auto Obj : InteractableObjects)
	{
		if (OwnerCharacter->GetDistanceTo(Obj) > RemoveRange)
		{
			ToRemoveActors.Add(Obj);
		}
	}

	// Delete Objects
	for (auto RemoveActor : ToRemoveActors)
	{
		RemoveInteractableObject(RemoveActor);
	}

	TArray<FHitResult> Hits;
	if (RangeTrace(ECollisionChannel::ECC_GameTraceChannel1, Hits))
	{
		for (FHitResult& Hit : Hits)
		{
			if (Hit.GetActor())
			{
				ICInterface_Interactable* OtherActor = Cast<ICInterface_Interactable>(Hit.GetActor());
				if (OtherActor && OtherActor->IsInteractable())
				{
					AddInteractableObject(Hit.GetActor());
				}
			}
		}
	}

	// Camera Detect

	if (ActionInteractTarget != nullptr)
	{
		if (OwnerCharacter->GetDistanceTo(ActionInteractTarget) > RemoveRange)
		{
			ActionInteractTarget = nullptr;
		}
	}
	FHitResult Hit;
	if (CameraTrace(ECollisionChannel::ECC_GameTraceChannel2, Hit))
	{
		if (OwnerCharacter->GetDistanceTo(ActionInteractTarget) > OwnerCharacter->GetDistanceTo(Hit.GetActor()))
		{
			ActionInteractTarget = Hit.GetActor();
		}
	}
}

AActor* UCInteractComponent::GetCurrentInteractTarget()
{
	ACPlayerController* PC = Cast<ACPlayerController>(OwnerCharacter->GetController());
	if (PC)
	{
		ACHUD* MyHud = Cast<ACHUD>(PC->GetHUD());

		if (MyHud)
		{
			UCHUDWidget* HUDWidget = MyHud->GetHUD();
			if (HUDWidget)
			{
				return HUDWidget->GetInteractTarget();
			}
		}
	}
	return nullptr;
}

bool UCInteractComponent::SetDetectInterval(float InTime)
{
	if (InTime < 0.0f) return false;

	DetectInterval = InTime;
	if (GetWorld()->GetTimerManager().TimerExists(DetectTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(DetectTimer);
	}
	//UKismetSystemLibrary::K2_SetTimer(this, "DetectInteractableObjects", DetectInterval, true);
	GetWorld()->GetTimerManager().SetTimer(DetectTimer, this, &UCInteractComponent::DetectInteractableObjects, DetectInterval, true);

	return true;
}

bool UCInteractComponent::SetDetectRange(float InRange)
{
	if (InRange < 0.0f) return false;

	DetectRange = InRange;
	return true;
}

bool UCInteractComponent::SetRemoveRange(float InRange)
{
	if (InRange < DetectRange) return false;

	RemoveRange = InRange;
	return true;
}

bool UCInteractComponent::RangeTrace(ECollisionChannel TraceChannel, TArray<FHitResult>& Hits)
{
	FVector Start = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetCameraComponent()->GetRelativeLocation();
	FVector End = Start + OwnerCharacter->GetCameraComponent()->GetForwardVector();

	//TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> Ignores;
	Ignores.Add(OwnerCharacter);

	UKismetSystemLibrary::SphereTraceMulti
	(
		GetWorld(),
		Start,
		End,
		DetectRange,
		UEngineTypes::ConvertToTraceType(TraceChannel),
		false,
		Ignores,
		EDrawDebugTrace::ForDuration,
		Hits,
		true
	);
	if (Hits.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trace Result: Hits.Num() = %d"), Hits.Num());

		return true;
	}
	else
	{
		return false;
	}
}

bool UCInteractComponent::CameraTrace(ECollisionChannel TraceChannel, FHitResult& Hit)
{
	FVector Start = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetCameraComponent()->GetRelativeLocation();
	FVector End = Start + OwnerCharacter->GetCameraComponent()->GetForwardVector();

	TArray<AActor*> Ignores;
	Ignores.Add(OwnerCharacter);

	return UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(),
		Start,
		End,
		UEngineTypes::ConvertToTraceType(TraceChannel),
		false,
		Ignores,
		EDrawDebugTrace::ForDuration,
		Hit,
		true
	);
}
