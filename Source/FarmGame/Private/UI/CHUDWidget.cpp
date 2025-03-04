#include "UI/CHUDWidget.h"
#include "UI/CItemNotification.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "UI/CInteractRowScroll.h"
#include "Characters/CPlayer.h"

void UCHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
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

void UCHUDWidget::OnInitPlayer(ACPlayer* InPlayer)
{
	if (!InPlayer) return;
	
	if (InteractRowScroll)
	{
		InteractRowScroll->OnInitPlyer(InPlayer);
	}
}
