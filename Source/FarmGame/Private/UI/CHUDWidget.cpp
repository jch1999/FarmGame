#include "UI/CHUDWidget.h"
#include "UI/CInteractRow.h"
#include "UI/CItemNotification.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"

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

	UCInteractRow* InteractRow=CreateWidget<UCInteractRow>(World, InteractRowClass);
	if (!InteractRow) return false;

	InteractRow->SetTarget(InActor);
	InteractRowScroll->AddChild(InteractRow);
	InteractRows.Add(InteractRow);

	if (InteractRows.Num() == 1)
	{
		InteractRows[0]->EnableTextOutline();
	}
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
	
	return BeforeLen != InteractRows.Num();
}

void UCHUDWidget::SetInteractIndex(int32 InIdx)
{
	if (InteractRows.Num() == 0)return;
	if (InIdx<0 || InIdx>=InteractRows.Num()) return;

	if (InteractIdx>=0 && InteractIdx < InteractRows.Num())
	{
		InteractRows[InteractIdx]->DisableTextOutline();
	}

	int32 PreviousIndex = InteractIdx;
	InteractIdx = InIdx;
	InteractRows[InIdx]->EnableTextOutline();

	UE_LOG(LogTemp, Warning, TEXT("SetInteractIndex PrevIndex: %d -> NewIndex: %d, NowSize: %d"), PreviousIndex, InteractIdx, InteractRows.Num());

}

int32 UCHUDWidget::GetInteractIndex()
{
	return InteractIdx;
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
