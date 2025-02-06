#include "UI/CItemNotification.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

void UCItemNotification::InitializeNotification(FName InItemName, int32 InItemAmount, UTexture2D* InItemIcon)
{
	Text_ItemName->SetText(FText::FromName(InItemName));
	Text_ItemAmount->SetText(FText::FromString(FString::FromInt(InItemAmount)));
	IMG_ItemIcon->SetBrushFromTexture(InItemIcon);
}
