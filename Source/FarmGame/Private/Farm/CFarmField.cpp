#include "Farm/CFarmField.h"
#include "Global.h"
#include "Components/CMoistureComponent.h"
#include "Components/CNutritionComponent.h"
#include "Components/CCultivationComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Crops/CBase_Crop.h"
#include "CGameModeBase.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UI/CFarmFieldWidget.h"

ACFarmField::ACFarmField()
{
	CHelpers::CreateSceneComponent(this, &MeshComp, "MeshComp", RootComponent);
	CHelpers::CreateActorComponent(this,&MoistureComp, "MoistureComp");
	CHelpers::CreateActorComponent(this, &NutritionComp, "NutritionComp");
	CHelpers::CreateActorComponent(this, &CultivationComp, "CultivationComp");
	CHelpers::CreateSceneComponent(this, &BoxComp, "BoxComp",RootComponent);
	CHelpers::CreateSceneComponent(this, &InfoWidgetComp, "InfoWidgetComp", MeshComp);

	// Set Widget Component
	if (InfoWidgetComp)
	{
		InfoWidgetComp->SetRelativeLocation(FVector(0, 0, 130.0f));
		InfoWidgetComp->SetWidgetSpace(EWidgetSpace::World);
		InfoWidgetComp->SetDrawSize(FVector2D(200.0f, 60.0f));

		CHelpers::GetClass<UCFarmFieldWidget>(&WidgetClass, "/Game/UI/WB_FarmFieldWidget");
		if (WidgetClass)
		{
			InfoWidgetComp->SetWidgetClass(WidgetClass);
		}
	}

	// Set Mesh Component
	UStaticMesh* MeshAsset;
	CHelpers::GetAsset(&MeshAsset, "/Game/Farm/Mesh/SM_FarmField");
	if (MeshAsset)
	{
		MeshComp->SetStaticMesh(MeshAsset);
	}

	// Set Box Component
	BoxComp->SetBoxExtent(FVector(64.0f, 64.0f, 96.0f));
	BoxComp->SetRelativeLocation(FVector(0.0f, 0.0f, 96.0f));
	BoxComp->SetCollisionProfileName(TEXT("ActionInteractObject"));

	SetInteractable();
	SetType(EInteractObjectType::FarmField);
}

void ACFarmField::BeginPlay()
{
	Super::BeginPlay();
	
	MoistureComp->AddMoisture(30.0f);
	MoistureComp->SetAutoReduceAmount(0.1f);
	MoistureComp->SetAutoReduceTimer(1.0f, true, 1.0f);
	MoistureComp->SetSafeRange(FVector2D(0.0f, 100.0f));

	NutritionComp->AddNutrition(50.0f);
	NutritionComp->SetAutoReduceAmount(0.05f);
	NutritionComp->SetAutoReduceTimer(1.0f, true, 1.0f);
	NutritionComp->SetSafeRange(FVector2D(0.0f, 100.0f));

	CultivationComp->AddCultivation(25.0f);
	CultivationComp->SetAutoReduceAmount(0.5f);
	CultivationComp->SetAutoReduceTimer(1.0f, true, 1.0f);

	// Widget
	/*UUserWidget* Widget = InfoWidgetComp->GetWidget();
	if (Widget)
	{
		InfoWidget = Cast<UCFarmFieldWidget>(Widget);
		if (InfoWidget)
		{
			InfoWidget->SetFarmField(this);
			InfoWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}*/
	InfoWidgetComp->SetVisibility(false);

	// Interact
	SetInteractable();

	// Set Dynamic Material
	FieldMaterial = UMaterialInstanceDynamic::Create(MeshComp->GetMaterial(0), nullptr);
}

void ACFarmField::SetInteractable()
{
	bInteractable = true;
}

void ACFarmField::SetUnInteractable()
{
	bInteractable = false;
}


void ACFarmField::SetType(EInteractObjectType InNewType)
{
	InteractType = InNewType;
}

void ACFarmField::Interact(AActor* OtherActor)
{
	/*UCFarmFieldWidget* FarmFieldWidget = GameMode->GetFarmFieldWidget();
	CheckNull(FarmFieldWidget);

	CheckFalse(FarmFieldWidget->IsAvailable());

	FarmFieldWidget->SetFarmField(this);
	FarmFieldWidget->AddToViewport();
	*/
	if (InfoWidget)
	{
		InfoWidgetComp->SetWorldRotation((OtherActor->GetActorForwardVector() * -1.0f).Rotation());
		InfoWidget->SetVisibility(ESlateVisibility::Visible);
	}
	InfoWidgetComp->SetVisibility(true);

	SetUnInteractable();
}

bool ACFarmField::PlantCrop(TSubclassOf<ACBase_Crop> InCropClass, const FTransform& InTM)
{
	CheckNullResult(InCropClass, false);
	CheckTrueResult(Crop != nullptr, false);

	Crop = GetWorld()->SpawnActorDeferred<ACBase_Crop>(InCropClass, InTM, this, nullptr);

	Crop->FinishSpawning(InTM);
	CheckNullResult(Crop, false);

	Crop->SetOwner(this);
	SetUnInteractable();

	return true;
}

bool ACFarmField::OnHovered()
{
	UE_LOG(LogTemp, Warning, TEXT("%s is Hovered!"), *GetInteractName().ToString());
	MeshComp->SetRenderCustomDepth(true);
	MeshComp->SetCustomDepthStencilValue(1);

	return false;
}

bool ACFarmField::OnUnhovered()
{
	UE_LOG(LogTemp, Warning, TEXT("%s is Unhovered!"), *GetInteractName().ToString());
	MeshComp->SetRenderCustomDepth(false);
	MeshComp->SetCustomDepthStencilValue(0);

	return false;
}
