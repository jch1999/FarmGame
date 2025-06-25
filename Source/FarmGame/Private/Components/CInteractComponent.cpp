#include "Components/CInteractComponent.h"
#include "GameFramework/Character.h"
#include "Controller/CPlayerController.h"
#include "Global.h"
#include "CHUD.h"
#include "UI/CHUDWidget.h"
#include "Characters/CPlayer.h"
#include "Camera/CameraComponent.h"
#include "UI/CInteractRowScroll.h"
#include "Item/CItem_Tool.h"

UCInteractComponent::UCInteractComponent()
{
	SetDetectRange(150.0f);
	SetRemoveRange(300.0f);
	SetDetectDistance(300.0f);
	SetRemoveDistance(500.0f);
	LastScrollTime = 0.0f;
	ScrollCooldown = 0.1f;
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
		OnInteractStart.Broadcast(OwnerCharacter);
	}
}

void UCInteractComponent::DoActionInteract()
{
	if (ActionInteractTarget == nullptr) return;
	ICInterface_Interactable* InteractActor = Cast<ICInterface_Interactable>(ActionInteractTarget);
	InteractActor->OnUnhovered();
	if (ACFarmField* FarmField = Cast<ACFarmField>(ActionInteractTarget))
	{
		if (ACItem_Tool* Tool = Cast<ACItem_Tool>(OwnerCharacter->GetCurretnEquippedItem()))
		{
			Tool->UseItem();
			ActionInteractTarget = nullptr;
			return;
		}
	}
	InteractActor->Interact(OwnerCharacter);

	ActionInteractTarget = nullptr;
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
		OnScrolled.Broadcast(InputValue > 0.0f);
	}
}

void UCInteractComponent::DetectInteractableObjects()
{
	if (!OwnerCharacter || !OwnerCharacter->IsInteractable()) return;

	// Range Detect
	// Delete Objects
	InteractableObjects.RemoveAll([&](AActor* Obj)
	{
			return !IsValid(Obj) || OwnerCharacter->GetDistanceTo(Obj) > RemoveRange;
	});

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
					InteractableObjects.AddUnique(Hit.GetActor());
				}
			}
		}
	}

	// Camera Detect
	if (IsValid(ActionInteractTarget) || OwnerCharacter->GetDistanceTo(ActionInteractTarget) > RemoveDistance)
	{
		ICInterface_Interactable* InteractObject = Cast<ICInterface_Interactable>(ActionInteractTarget);
		InteractObject->OnUnhovered();

		ActionInteractTarget = nullptr;
	}

	FHitResult Hit;
	if (CameraTrace(ECollisionChannel::ECC_GameTraceChannel2, Hit))
	{
		if (IsValid(ActionInteractTarget))
		{
			if (OwnerCharacter->GetDistanceTo(ActionInteractTarget) > OwnerCharacter->GetDistanceTo(Hit.GetActor()))
			{
				if (ICInterface_Interactable* InteractObject = Cast<ICInterface_Interactable>(ActionInteractTarget))
				{
					InteractObject->OnUnhovered();
					ActionInteractTarget = nullptr;
				}
				//UE_LOG(LogTemp, Warning, TEXT("%s is undetected!"), *(InteractObject->GetInteractName().ToString()));
			}
			else
			{
				return;
			}
		}

		ActionInteractTarget = Hit.GetActor();
		if (ICInterface_Interactable* InteractObject = Cast<ICInterface_Interactable>(ActionInteractTarget))
		{
			InteractObject->OnHovered();
		}
		//UE_LOG(LogTemp, Warning, TEXT("%s is detected!"), *(InteractObject->GetInteractName().ToString()));
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("Nothing Detected."));
	}

	OnInteractInfoUpdated.Broadcast(InteractableObjects);
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
				return HUDWidget->GetInteractRowScroll()->GetInteractTarget();
			}
		}
	}
	return nullptr;
}

AActor* UCInteractComponent::GetActionInteractTarget()
{
	return ActionInteractTarget;
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

bool UCInteractComponent::SetDetectDistance(float InDistance)
{
	if (InDistance < 0.0f) return false;

	DetectDistance = InDistance;
	return true;
}

bool UCInteractComponent::SetRemoveDistance(float InDistance)
{
	if (InDistance < 0.0f) return false;

	RemoveDistance = InDistance;
	return true;
}

bool UCInteractComponent::RangeTrace(ECollisionChannel TraceChannel, TArray<FHitResult>& Hits)
{
	FVector Start = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetCameraComponent()->GetRelativeLocation();
	FVector End = Start + OwnerCharacter->GetCameraComponent()->GetForwardVector();


	// 트레이스 파라미터 설정
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(OwnerCharacter);
	// 충돌 반경 설정
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(DetectRange);
	GetWorld()->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, TraceChannel, SphereShape, TraceParams);

	/*TArray<AActor*> Ignores;
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
	);*/

	if (Hits.Num() > 0)
	{
		/*for (const FHitResult& Hit : Hits)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.0f, 8, FColor::Red, false, 2.0f);
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *Hit.GetActor()->GetName());
		}*/
		//UE_LOG(LogTemp, Warning, TEXT("Trace Result: Hits.Num() = %d"), Hits.Num());

		return true;
	}
	else
	{
		return false;
	}
}

bool UCInteractComponent::CameraTrace(ECollisionChannel TraceChannel, FHitResult& Hit)
{
	// UE_LOG(LogTemp, Warning, TEXT("CameraTrace Start!"));
	FVector Start = OwnerCharacter->GetCameraComponent()->GetComponentLocation();
	FVector End = Start + OwnerCharacter->GetCameraComponent()->GetForwardVector() * DetectDistance;

	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);
	/*TArray<AActor*> Ignores;
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
	);*/
	// Sphere Trace로 변경 (반지름 10.0f)
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(10.0f);

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(OwnerCharacter);

	// Sweep을 사용하여 감지
	return GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, TraceChannel, SphereShape, TraceParams);
}
