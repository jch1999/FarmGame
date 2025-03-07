#include "UI/CHUDWidget.h"
#include "UI/CItemNotification.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "UI/CInteractRowScroll.h"
#include "UI/CQuickSlotBarWidget.h"
#include "Characters/CPlayer.h"
#include "Components/CInventoryComponent.h"
#include "Global.h"

bool UCHUDWidget::Initialize()
{
	bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;

	if (!ItemNotificationClass)
	{
		CHelpers::GetClassDynamic(&ItemNotificationClass, "/Game/UI/WB_ItemNotification.WB_ItemNotification_C");
	}
	return true;
}

void UCHUDWidget::OnItemAdded(FName InItemName, int32 InItemAmount, UTexture2D* InItemIcon)
{
	AddItemNotification(InItemName, InItemAmount, InItemIcon);
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
	ItemNotification->AddToViewport();
	return true;
}

void UCHUDWidget::OnInitPlayer(ACPlayer* InPlayer)
{
	if (!InPlayer) return;
	
	if (InteractRowScroll)
	{
		InteractRowScroll->OnInitPlyer(InPlayer);
		QuickSlotBar->OnInitPlayer(InPlayer);
		InPlayer->GetInventoryComponent()->OnItemAdded.AddDynamic(this, &UCHUDWidget::OnItemAdded);
	}
}
