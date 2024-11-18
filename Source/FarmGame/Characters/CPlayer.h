#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "CPlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UCAttributeComponent;
class UCOptionComponent;

UCLASS()
class FARMGAME_API ACPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void Move(const FInputActionInstance& InInstance);
	void Look(const FInputActionInstance& InInstance);

protected:
	UPROPERTY(VisibleAnywhere, Category="Input")
	UInputMappingContext* DefaultContext;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCAttributeComponent* AttributeComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCOptionComponent* OptionComp;
};
