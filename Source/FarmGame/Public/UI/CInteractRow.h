// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CInteractRow.generated.h"

class UTextBlock;

UCLASS()
class FARMGAME_API UCInteractRow : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SetTarget(AActor* InActor);
	UFUNCTION()
	AActor* GetTarget();

	UFUNCTION()
	void EnableTextOutline();
	UFUNCTION()
	void DisableTextOutline();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* NameText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<AActor> TargetActor;
};
