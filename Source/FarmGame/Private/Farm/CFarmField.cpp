#include "Farm/CFarmField.h"
#include "Global.h"
#include "Components/CMoistureComponent.h"
#include "Components/CNutritionComponent.h"
#include "Components/CCultivationComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "Crops/CBase_Crop.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UI/CFarmFieldWidget.h"
#include "Characters/CPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Controller/CPlayerController.h"
#include "CGameInstance.h"

ACFarmField::ACFarmField()
{
	CHelpers::CreateSceneComponent(this, &MeshComp, "MeshComp", RootComponent);
	CHelpers::CreateActorComponent(this,&MoistureComp, "MoistureComp");
	CHelpers::CreateActorComponent(this, &NutritionComp, "NutritionComp");
	CHelpers::CreateActorComponent(this, &CultivationComp, "CultivationComp");
	CHelpers::CreateSceneComponent(this, &BoxComp, "BoxComp",RootComponent);
	CHelpers::CreateSceneComponent(this, &CameraComp, "CameraComp", RootComponent);

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

	// Set Camera Component
	CameraComp->SetRelativeLocation(FVector(-135.0f, 0.0f, 200.0f));
	CameraComp->SetRelativeRotation(FQuat(FRotator(-30.0f, 0.0f, 0.0f)));

	SetInteractable();
	SetType(EInteractObjectType::FarmField);

	PrimaryActorTick.bCanEverTick = false;

}

void ACFarmField::BeginPlay()
{
	Super::BeginPlay();
	
	MoistureComp->SetSafeRange(FVector2D(0.0f, 100.0f));
	MoistureComp->AddMoisture(30.0f);
	MoistureComp->SetAutoReduceAmount(0.1f);
	MoistureComp->SetAutoReduceTimer(1.0f, true, 1.0f);
	UE_LOG(LogTemp, Warning, TEXT("[BeginPlay] Moisture After Add: %.1f"), MoistureComp->GetCurrentValue());

	NutritionComp->SetSafeRange(FVector2D(0.0f, 100.0f));
	NutritionComp->AddNutrition(50.0f);
	NutritionComp->SetAutoReduceAmount(0.0f);
	NutritionComp->SetAutoReduceTimer(1.0f, true, 1.0f);
	UE_LOG(LogTemp, Warning, TEXT("[BeginPlay] Nutrition After Add: %.1f"), NutritionComp->GetCurrentValue());

	CultivationComp->AddCultivation(50.0f);
	CultivationComp->SetAutoReduceAmount(0.1f);
	CultivationComp->SetAutoReduceTimer(1.0f, true, 1.0f);
	UE_LOG(LogTemp, Warning, TEXT("[BeginPlay] Cultivation After Add: %.1f"), CultivationComp->GetCurrentValue());
	// Interact
	SetInteractable();

	// Set Dynamic Material
	FieldMaterial = UMaterialInstanceDynamic::Create(MeshComp->GetMaterial(0), nullptr);
}

void ACFarmField::SetInteractable()
{
	if (IsValid(Crop) && Crop->IsHarvestable())
	{
		Crop->SetInteractable();
	}
	else
	{
		bInteractable = true;
		BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ACFarmField::SetUnInteractable()
{
	if (IsValid(Crop) && Crop->IsHarvestable())
	{
		Crop->SetUnInteractable();
	}
	bInteractable = false;
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACFarmField::SetDelayedInteractable(float DelayTime)
{
	if (GetWorld()->GetTimerManager().TimerExists(InteractTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractTimer);
	}
	if (IsValid(Crop) && Crop->IsHarvestable())
	{
		UE_LOG(LogTemp, Warning, TEXT("Crop is harvestable. FarmField shouldn't be interacted."));
		return;
	}
	GetWorld()->GetTimerManager().SetTimer(InteractTimer, this, &ACFarmField::SetInteractable, DelayTime, false);
}

void ACFarmField::SetDelayedUninteractable(float DelayTime)
{
	if (GetWorld()->GetTimerManager().TimerExists(InteractTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractTimer);
	}
	GetWorld()->GetTimerManager().SetTimer(InteractTimer, this, &ACFarmField::SetUnInteractable, DelayTime, false);
}


void ACFarmField::SetType(EInteractObjectType InNewType)
{
	InteractType = InNewType;
}

void ACFarmField::Interact(AActor* OtherActor)
{
	if (!IsInteractable()) return;

	if (ACPlayer* Player = Cast<ACPlayer>(OtherActor))
	{
		float WidgetDelay = 0.0f;
		if (APlayerController* PC = Cast<APlayerController>(Player->GetController()))
		{
			if (ACPlayerController* MyPC = Cast<ACPlayerController>(PC))
			{
				CachedPlayerController = MyPC;
				CachedPlayerController->SwitchCamera(this);
				WidgetDelay = CachedPlayerController->GetCameraMoveTime();
			}
		}

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACFarmField::ShowFarmFieldWidget, WidgetDelay, false);
	}

	SetUnInteractable();
}

bool ACFarmField::PlantCrop(TSubclassOf<ACBase_Crop> InCropClass, const FVector& RelativeOffset)
{
	CheckTrueResult(Crop != nullptr, false);
	CheckNullResult(InCropClass, false);
	FTransform CropTM = FTransform(FRotator::ZeroRotator, GetActorLocation() + RelativeOffset);
	ACBase_Crop* NewCrop = GetWorld()->SpawnActorDeferred<ACBase_Crop>(InCropClass, CropTM, this, nullptr);
	if (!NewCrop) return false;

	NewCrop->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	NewCrop->SetActorRelativeLocation(RelativeOffset);
	NewCrop->SetOwner(this);
	NewCrop->SetFarmField(this);
	Crop = NewCrop;
	NewCrop->FinishSpawning(CropTM); 

	//SetUnInteractable();

	return true;
}

void ACFarmField::FarmFieldOn()
{
	SetInteractable();
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACFarmField::FarmFieldOff()
{
	SetUnInteractable();
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACFarmField::ShowFarmFieldWidget()
{
	if (CachedPlayerController)
	{
		CachedPlayerController->ShowFarmWidget(this);
	}
}

bool ACFarmField::OnHovered()
{
	//UE_LOG(LogTemp, Warning, TEXT("%s is Hovered!"), *GetInteractName().ToString());
	MeshComp->SetRenderCustomDepth(true);
	MeshComp->SetCustomDepthStencilValue(1);

	return false;
}

bool ACFarmField::OnUnhovered()
{
	//UE_LOG(LogTemp, Warning, TEXT("%s is Unhovered!"), *GetInteractName().ToString());
	MeshComp->SetRenderCustomDepth(false);
	MeshComp->SetCustomDepthStencilValue(0);

	return false;
}
