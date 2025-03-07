#include "UI/CItemNotification.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

void UCItemNotification::NativeConstruct()
{
	Super::NativeConstruct(); 
	PlayWidgetAnimation();
}

void UCItemNotification::InitializeNotification(FName InItemName, int32 InItemAmount, UTexture2D* InItemIcon)
{
	Text_ItemName->SetText(FText::FromName(InItemName));
	Text_ItemAmount->SetText(FText::FromString(FString::FromInt(InItemAmount)));
	IMG_ItemIcon->SetBrushFromTexture(InItemIcon);
}

void UCItemNotification::PlayWidgetAnimation()
{
	if (AppearFromLeftAnim)
	{
		PlayAnimation(AppearFromLeftAnim);

		GetWorld()->GetTimerManager().SetTimer(ReverseTimerHandle, this, &UCItemNotification::ReverseWidgetAnimation, LifeTime, false);
	}
}

void UCItemNotification::ReverseWidgetAnimation()
{
	if (AppearFromLeftAnim)
	{
		PlayAnimationReverse(AppearFromLeftAnim);

		GetWorld()->GetTimerManager().SetTimer(RemoveTimerHandle, this, &UCItemNotification::RemoveWidget, LifeTime, false);
	}
}

void UCItemNotification::RemoveWidget()
{
	RemoveFromParent();
}
