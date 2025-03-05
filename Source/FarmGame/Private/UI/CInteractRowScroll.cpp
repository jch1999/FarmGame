#include "UI/CInteractRowScroll.h"
#include "Global.h"
#include "UI/CInteractRow.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Components/PanelSlot.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/Image.h"
#include "Components/CInteractComponent.h"
#include "UI/CInteractRow.h"
#include "Characters/CPlayer.h"

void UCInteractRowScroll::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CHelpers::GetClassDynamic(&InteractRowClass, "/Game/UI/WB_InteractRow.WB_InteractRow_C");
	InteractIdx = 0;
}

void UCInteractRowScroll::UpInteractIndex()
{
	if (InteractRows.Num() <= 0)return;

	if (InteractIdx < InteractRows.Num()) InteractRows[InteractIdx]->OnUnSelected();

	int32 PreviousIndex = InteractIdx;
	InteractIdx = (InteractIdx + 1) % InteractRows.Num();
	InteractRows[InteractIdx]->OnSelected();

	UE_LOG(LogTemp, Warning, TEXT("SetInteractIndex PrevIndex: %d -> NewIndex: %d, NowSize: %d"), PreviousIndex, InteractIdx, InteractRows.Num());
	EnsureVisibleInteractRow();
}

void UCInteractRowScroll::DownInteractIndex()
{
	if (InteractRows.Num() <= 0)return;

	if (InteractIdx < InteractRows.Num()) InteractRows[InteractIdx]->OnUnSelected();

	int32 PreviousIndex = InteractIdx;
	InteractIdx = (InteractIdx - 1 + InteractRows.Num()) % InteractRows.Num();

	InteractRows[InteractIdx]->OnSelected();

	UE_LOG(LogTemp, Warning, TEXT("SetInteractIndex PrevIndex: %d -> NewIndex: %d, NowSize: %d"), PreviousIndex, InteractIdx, InteractRows.Num());
	EnsureVisibleInteractRow();
}

int32 UCInteractRowScroll::GetInteractIndex()
{
	return InteractIdx;
}

AActor* UCInteractRowScroll::GetInteractTarget()
{
	if (InteractRows.Num() <= 0) return nullptr;
	return InteractRows[InteractIdx]->GetTarget();
}

void UCInteractRowScroll::EnsureVisibleInteractRow()
{
	if (!InteractRowScroll || InteractRows.Num() == 0) return;
	if (InteractIdx < 0 || InteractIdx >= InteractRows.Num()) return;

	InteractRowScroll->ScrollWidgetIntoView(InteractRows[InteractIdx]);
	//GetWorld()->GetTimerManager().ClearTimer(InteractScrollLerpTimer);
	//GetWorld()->GetTimerManager().SetTimer(InteractScrollLerpTimer, this, &UCHUDWidget::ScrollLerp, 0.02f, true, 0.0f);
}

void UCInteractRowScroll::ScrollLerp()
{
	UCInteractRow* SelectedRow = InteractRows[InteractIdx];
	if (!SelectedRow)
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractScrollLerpTimer);
		return;
	}

	float CurrentOffset = InteractRowScroll->GetScrollOffset();
	float TargetOffset = GetChildWidgetOffset(InteractRowScroll, SelectedRow);
	if (FMath::IsNearlyZero(TargetOffset - CurrentOffset, 0.01f))
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractScrollLerpTimer);
		return;
	}
	InteractRowScroll->SetScrollOffset(FMath::Lerp(CurrentOffset, TargetOffset, 0.2f));
}

float UCInteractRowScroll::GetChildWidgetOffset(UScrollBox* ScrollBox, UWidget* TargetWidget)
{
	if (!ScrollBox || !TargetWidget) return -1.0f;

	// 자식 위젯 리스트 가져오기
	TArray<UWidget*> Children = ScrollBox->GetAllChildren();

	float Offset = 0.0f;

	for (UWidget* Child : Children)
	{
		if (Child == TargetWidget)
		{
			return Offset;
		}

		// Slot 정보를 가져와서 Offset 계산
		UPanelSlot* PanelSlot = Child->Slot;
		if (UScrollBoxSlot* ScrollSlot = Cast<UScrollBoxSlot>(PanelSlot))
		{
			FMargin SlotPadding = ScrollSlot->GetPadding();
			Offset += SlotPadding.Top + SlotPadding.Bottom;
		}
	}

	return Offset;
}

void UCInteractRowScroll::OnInitPlyer(ACPlayer* InPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("UCInteractRowScroll::OnInitPlyer"));
	if (InPlayer)
	{
		if (UCInteractComponent* InteractComponent = InPlayer->GetInteractComponent())
		{
			InteractComponent->OnInteractInfoUpdated.AddDynamic(this, &UCInteractRowScroll::UpdateInteractInfo);
			InteractComponent->OnScrolled.AddDynamic(this, &UCInteractRowScroll::UpdateScroll);
			InteractComponent->OnInteractStart.AddDynamic(this, &UCInteractRowScroll::DoInteract);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Can't find InteractComp! In UCInteractRowScroll::OnInitPlyer"));
		}
	}
}

void UCInteractRowScroll::UpdateScroll(bool IsUp)
{
	if (IsUp)
	{
		UpInteractIndex();
	}
	else
	{
		DownInteractIndex();
	}
}

void UCInteractRowScroll::UpdateInteractInfo(const TArray<AActor*>& InteractableObjects)
{
	// Reomve invalid InteractRow
	InteractRows.RemoveAll([&](UCInteractRow* InteractRow)
		{
			if (!IsValid(InteractRow->GetTarget()) || !InteractableObjects.Contains(InteractRow->GetTarget()))
			{
				InteractRowScroll->RemoveChild(InteractRow);
				return true;
			}
			return false;
		});

	TMap<AActor*, UCInteractRow*> InteractRowMap;
	for (UCInteractRow* InteractRow : InteractRows)
	{
		InteractRowMap.Add(InteractRow->GetTarget(), InteractRow);
	}
	
	// 새로운 InteractRow 생성 및 추가 코드
	if (!InteractRowClass) 
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractRowClass is null!"));
		return;

	}
	UWorld* World = GetWorld();
	if (!World) return;
	
	for (auto InteractableObject : InteractableObjects)
	{
		UE_LOG(LogTemp, Warning, TEXT("Checking InteractableObject: %s"), *InteractableObject->GetName());
		if (!InteractRowMap.Contains(InteractableObject))
		{
			UE_LOG(LogTemp, Warning, TEXT("Creating InteractRow for: %s"), *InteractableObject->GetName());
			UCInteractRow* InteractRow = CreateWidget<UCInteractRow>(World, InteractRowClass);
			UE_LOG(LogTemp, Warning, TEXT("Create InteractRow! UCInteractRowScroll::UpdateInteractInfo"));

			if (InteractRow)
			{
				UE_LOG(LogTemp, Warning, TEXT("Add InteractRow! UCInteractRowScroll::UpdateInteractInfo"));
				InteractRow->SetTarget(InteractableObject);
				InteractRowScroll->AddChild(InteractRow);
				InteractRows.Add(InteractRow);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to create InteractRow!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("InteractRow already exists for: %s"), *InteractableObject->GetName());
		}
	}

	InteractIdx = FMath::Clamp(InteractIdx, 0, InteractRows.Num() - 1);
	if (InteractRows.Num() > 0 && IsValid(InteractRows[InteractIdx]))
	{
		InteractRows[InteractIdx]->OnSelected();
	}
	MouseWheelIcon->SetVisibility(InteractRows.Num() > 0 ? ESlateVisibility::Visible: ESlateVisibility::Hidden);

	EnsureVisibleInteractRow();
}

void UCInteractRowScroll::DoInteract(ACPlayer* InteractCauser)
{
	if (InteractRows.Num() > 0)
	{
		if (AActor* Target = GetInteractTarget())
		{
			if (ICInterface_Interactable* InteractActor = Cast<ICInterface_Interactable>(Target))
			{
				InteractActor->Interact(InteractCauser);
			}
	}
	}
}
