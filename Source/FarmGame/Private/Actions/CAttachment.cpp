#include "Actions/CAttachment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/ShapeComponent.h"

ACAttachment::ACAttachment()
{
	CHelpers::CreateSceneComponent(this, &RootComp, "RootComp");

	CHelpers::CreateSceneComponent(this, &MeshComp, "MeshComp", RootComp);
}

// Called when the game starts or when spawned
void ACAttachment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	GetComponents<UShapeComponent>(ShapeComponents);

	for (const auto& Comp : ShapeComponents)
	{
		Comp->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment::OnComponentBeginOverlap);
		Comp->OnComponentEndOverlap.AddDynamic(this, &ACAttachment::OnComponentEndOverlap);
	}
	OffCollision();

	Super::BeginPlay();
}

void ACAttachment::ActorAttachTo(FName InSocketName)
{
	AttachToComponent
	(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative,true),
		InSocketName
	);
}

void ACAttachment::ComponentAttachTo(USceneComponent* InComp, FName InSocketName)
{
	InComp->AttachToComponent
	(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative,true),
		InSocketName
	);
}

void ACAttachment::OnCollision()
{
	for (const auto& Comp : ShapeComponents)
	{
		Comp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ACAttachment::OffCollision()
{
	for (const auto& Comp : ShapeComponents)
	{
		Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor);

	if (OwnerCharacter)
	{
		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, OtherActor);
	}
}

void ACAttachment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OwnerCharacter)
	{
		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, OtherActor);
	}
}

