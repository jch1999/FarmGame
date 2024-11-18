#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActionComponent.generated.h"

UENUM(BlueprintType)
enum class EActionType : uint8
{
	Unarmed, RightHand, LeftHand, TwoHand, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActionTypeChanged, EActionType, InPrevType, EActionType, InNewType);

class UCActionData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMGAME_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCActionComponent();

protected:
	virtual void BeginPlay() override;

public:
	void PrimaryAction();
	
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsUnarmedMode() const { return Type == EActionType::Unarmed; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsRightHandMode() const { return Type == EActionType::RightHand; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsLeftHandMode() const { return Type == EActionType::LeftHand; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsTwoHandMode() const { return Type == EActionType::TwoHand; }

	/*UFUNCTION(BlueprintPure)
	FORCEINLINE UCActionData* GetCurrentDataAsset() const { return DataAssets[(int32)Type]; }*/

	void SetUnarmedMode();
	void SetRightHandMode();
	void SetLeftHandMode();
	void SetTwoHandMode();

private:
	void SetMode(EActionType InNewType);
	void ChangeType(EActionType InNewType);

public:
	UPROPERTY(BlueprintAssignable)
	FActionTypeChanged OnActionTypeChanged;

//protected:
//	UPROPERTY(EditDefaultsOnly, Category = "DataAsset")
//	UCActionData* DataAssets[(int32)EActionType::Max];

private:	
	EActionType Type;
};
