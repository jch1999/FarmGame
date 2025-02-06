#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CActionData.h"
#include "CDoAction.generated.h"

class ACharacter;
class UCStateComponent;
class UCAttributeComponent;
UCLASS()
class FARMGAME_API ACDoAction : public AActor
{
	GENERATED_BODY()
	
public:	
	ACDoAction();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	void SetDatas(TArray<FActionData>& InDatas);

	virtual void PrimaryAction() {};
	virtual void Begin_PrimaryAction() {};
	virtual void End_PrimaryAction() {};

	virtual void Begin_SecondaryAction() {};
	virtual void End_SecondaryAction() {};

public:
	UFUNCTION()
	virtual void OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter) {};

	UFUNCTION()
	virtual void OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter) {};
	// Attachment->Delegate.AddDynamic(DoAction, Func)

protected:
	UPROPERTY(BlueprintReadOnly, Category = "DoAction")
	ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "DoAction")
	UCStateComponent* StateComp;

	UPROPERTY(BlueprintReadOnly, Category = "DoAction")
	UCAttributeComponent* AttributeComp;

protected:
	TArray<FActionData> Datas;
};
