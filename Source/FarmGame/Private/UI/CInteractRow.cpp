#include "UI/CInteractRow.h"
#include "Utilities/CHelpers.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Interfaces/CInterface_Interactable.h"
void UCInteractRow::SetTarget(AActor* InActor)
{
	CheckNull(InActor);

	TargetActor = InActor;
	ICInterface_Interactable* Interact = Cast<ICInterface_Interactable>(InActor);
	//NameText->SetText(FText::FromName(Interact->GetInteractName()));
	NameText->SetText(FText::FromString(InActor->GetActorLabel()));
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