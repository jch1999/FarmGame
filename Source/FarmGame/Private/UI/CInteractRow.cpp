#include "UI/CInteractRow.h"
#include "Utilities/CHelpers.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "Interfaces/Interactable.h"
#include "CGameInstance.h"
#include "Global.h"

void UCInteractRow::SetTarget(AActor* InActor)
{
	CheckNull(InActor);

	TargetActor = InActor;
	IInteractable* Interact = Cast<IInteractable>(InActor);
	NameText->SetText(FText::FromName(Interact->GetInteractName()));
	//NameText->SetText(FText::FromString(InActor->GetActorLabel()));

	if (UCGameInstance* GI = Cast<UCGameInstance>(GetGameInstance()))
	{
		if (const TOptional<FInteractAssetData>& InteractAssetDataOpt = GI->GetInteractAssetData(Interact->GetType()))
		{
			if (InteractAssetDataOpt.IsSet())
			{
				const FInteractAssetData& InteractAssetData = InteractAssetDataOpt.GetValue();
				UTexture2D* InteractIconTex;
				CHelpers::GetAssetDynamic(&InteractIconTex, InteractAssetData.InteractIconTextureRef);
				if (IsValid(InteractIconTex))
				{
					InteractTypeIconImage->SetBrushFromTexture(InteractIconTex);
				}
			}
		}
	}
}

AActor* UCInteractRow::GetTarget()
{
	return TargetActor.Get();
}

void UCInteractRow::OnSelected()
{
	EnableTextOutline();
	InteractKeyOverlay->SetVisibility(ESlateVisibility::Visible);
}

void UCInteractRow::OnUnSelected()
{
	DisableTextOutline();
	InteractKeyOverlay->SetVisibility(ESlateVisibility::Hidden);
}

void UCInteractRow::EnableTextOutline()
{
	FFontOutlineSettings OutlineSetting(5, FLinearColor::Green);
	FSlateFontInfo FontInfo = NameText->GetFont();
	FontInfo.OutlineSettings = OutlineSetting;
	NameText->SetFont(FontInfo);
}

void UCInteractRow::DisableTextOutline()
{
	FFontOutlineSettings OutlineSetting(0);
	FSlateFontInfo FontInfo = NameText->GetFont();
	FontInfo.OutlineSettings = OutlineSetting;
	NameText->SetFont(FontInfo);
}