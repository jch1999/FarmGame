#include "UI/CHUDWidget.h"
#include "UI/CInteractRow.h"
#include "UI/CItemNotification.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Components/PanelSlot.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/Image.h"

void UCHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InteractIdx = 0;
}

bool UCHUDWidget::AddInteractRow(AActor* InActor)
{
	if (!InteractRowClass) return false;

	UWorld* World = GetWorld();
	if (!World) return false;

	// 이미 존재하는 경우 추가하지 않음
	for (UCInteractRow* ExistingRow : InteractRows)
	{
		if (ExistingRow->GetTarget() == InActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("AddInteractRow: Target already exists -> %s"), *InActor->GetName());
			return false;
		}
	}

	UCInteractRow* InteractRow=CreateWidget<UCInteractRow>(World, InteractRowClass);
	if (!InteractRow) return false;

	InteractRow->SetTarget(InActor);
	InteractRowScroll->AddChild(InteractRow);
	InteractRows.Add(InteractRow);

	if (InteractRows.Num() == 1)
	{
		InteractRows[0]->OnSelected();
		MouseWheelIcon->SetVisibility(ESlateVisibility::Visible);
	}
	EnsureVisibleInteractRow();

	return true;
}

bool UCHUDWidget::RemoveInteractRow(AActor* InActor)
{
	int32 BeforeLen = InteractRows.Num();
	
	InteractRows.RemoveAll([&](UCInteractRow* InteractRow)
		{
			if (InteractRow->GetTarget() == InActor)
			{
				InteractRowScroll->RemoveChild(InteractRow);
				InteractRow->RemoveFromParent();
				return true;  // 삭제 조건 충족 시 삭제
			}
			return false;  // 삭제 조건 충족하지 않으면 유지
		});
	
	// 인덱스 보정
	int32 AfterLen = InteractRows.Num();
	if (InteractIdx >= AfterLen)
	{
		InteractIdx = FMath::Max(0, AfterLen - 1);
	}

	if (InteractRows.Num() <= 0)
	{
		MouseWheelIcon->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		InteractRows[InteractIdx]->OnSelected();
	}
	EnsureVisibleInteractRow();

	return BeforeLen != InteractRows.Num();
}

void UCHUDWidget::SetInteractIndex(int32 InIdx)
{
	UE_LOG(LogTemp, Warning, TEXT("SetInteractIndex Called: InIdx = %d, InteractIdx Before = %d"), InIdx, InteractIdx);
	if (InteractRows.Num() == 0) 
	{
		InteractIdx = 0;
		return;
	}

	if (InIdx<0 || InIdx>=InteractRows.Num()) return;

	if (InteractIdx < InteractRows.Num())
	{
		InteractRows[InteractIdx]->OnUnSelected();
	}

	int32 PreviousIndex = InteractIdx;
	InteractRows[InIdx]->OnSelected();
	InteractIdx = InIdx;

	UE_LOG(LogTemp, Warning, TEXT("SetInteractIndex PrevIndex: %d -> NewIndex: %d, NowSize: %d"), PreviousIndex, InteractIdx, InteractRows.Num());

}

void UCHUDWidget::UpInteractIndex()
{
	if (InteractRows.Num() <= 0)return;

	if (InteractIdx < InteractRows.Num())
	{
		InteractRows[InteractIdx]->OnUnSelected();
	}

	int32 PreviousIndex = InteractIdx;
	++InteractIdx %= InteractRows.Num();
	InteractRows[InteractIdx]->OnSelected();

	UE_LOG(LogTemp, Warning, TEXT("SetInteractIndex PrevIndex: %d -> NewIndex: %d, NowSize: %d"), PreviousIndex, InteractIdx, InteractRows.Num());
	EnsureVisibleInteractRow();
}

void UCHUDWidget::DownInteractIndex()
{
	if (InteractRows.Num() <= 0)return;

	if (InteractIdx < InteractRows.Num())
	{
		InteractRows[InteractIdx]->OnUnSelected();
	}

	int32 PreviousIndex = InteractIdx;
	--InteractIdx;
	if (InteractIdx < 0)
	{
		InteractIdx = InteractRows.Num() - 1;
	}

	InteractRows[InteractIdx]->OnSelected();

	UE_LOG(LogTemp, Warning, TEXT("SetInteractIndex PrevIndex: %d -> NewIndex: %d, NowSize: %d"), PreviousIndex, InteractIdx, InteractRows.Num());
	EnsureVisibleInteractRow();
}

int32 UCHUDWidget::GetInteractIndex()
{
	return InteractIdx;
}

AActor* UCHUDWidget::GetInteractTarget()
{
	if (InteractRows.Num() <= 0)
	{
		return nullptr;
	}
	AActor* TargetACtor = InteractRows[InteractIdx]->GetTarget();
	return TargetACtor;
}

void UCHUDWidget::EnsureVisibleInteractRow()
{
	if (!InteractRowScroll || InteractRows.Num() == 0) return;

	if (InteractIdx < 0 || InteractIdx >= InteractRows.Num()) return;

	UCInteractRow* SelectedRow = InteractRows[InteractIdx];
	if (!SelectedRow) return;

	InteractRowScroll->ScrollWidgetIntoView(SelectedRow);
	//GetWorld()->GetTimerManager().ClearTimer(InteractScrollLerpTimer);
	//GetWorld()->GetTimerManager().SetTimer(InteractScrollLerpTimer, this, &UCHUDWidget::ScrollLerp, 0.02f, true, 0.0f);
}

void UCHUDWidget::ScrollLerp()
{
	UCInteractRow* SelectedRow = InteractRows[InteractIdx];
	if (!SelectedRow)
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractScrollLerpTimer);
		return;
	}

	float CurrentOffset = InteractRowScroll->GetScrollOffset();
	float TargetOffset = GetChildWidgetOffset(InteractRowScroll,SelectedRow);
	if (FMath::IsNearlyZero(TargetOffset - CurrentOffset, 0.01f))
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractScrollLerpTimer);
		return;
	}
	InteractRowScroll->SetScrollOffset(FMath::Lerp(CurrentOffset, TargetOffset, 0.2f));
}

bool UCHUDWidget::AddItemNotification(FName InItemName, int32 InItemAmount, UTexture2D* InItemIcon)
{
	if (!ItemNotificationClass) return false;

	UWorld* World = GetWorld();
	if (!World) return false;

	UCItemNotification* ItemNotification = CreateWidget<UCItemNotification>(World, ItemNotificationClass);
	if (!ItemNotification) return false;

	ItemNotification->InitializeNotification(InItemName, InItemAmount, InItemIcon);
	VB_ItemNotifications->AddChild(ItemNotification);
	return true;
}

float UCHUDWidget::GetChildWidgetOffset(UScrollBox* ScrollBox, UWidget* TargetWidget)
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
			Offset += ScrollSlot->Padding.Top + ScrollSlot->Padding.Bottom;
		}
	}

	return Offset;
}
