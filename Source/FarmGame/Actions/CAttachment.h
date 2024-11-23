#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

class ACharacter;
class UStaticMeshComponent;
class UStaticMesh;
class UShapeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentOverlap, AActor*, InPerfomer, AActor*, InCauser, AActor*, InOtherActor);

UCLASS()
class FARMGAME_API ACAttachment : public AActor
{
	GENERATED_BODY()
	
public:	
	ACAttachment();

protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION(BlueprintCallable)
	void ActorAttachTo(FName InSocketName);

	UFUNCTION(BlueprintCallable)
	void ComponentAttachTo(USceneComponent* InComp, FName InSocketName);

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnEquip();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUnequip();

public:
	void OnCollision();
	void OffCollision();

public:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UPROPERTY(BlueprintAssignable)
	FAttachmentOverlap OnAttachmentBeginOverlap;

	UPROPERTY(BlueprintAssignable)
	FAttachmentOverlap OnAttachmentEndOverlap;

protected:
	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	USceneComponent* RootComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(BlueprintReadOnly)
	ACharacter* OwnerCharacter;

private:
	TArray<UShapeComponent*> ShapeComponents;
};
