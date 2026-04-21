// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/MechBaseAnimInstance.h"
#include "MechCharacterAnimInstance.generated.h"

class AMechBaseCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class PALACEMANAGER_API UMechCharacterAnimInstance : public UMechBaseAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);
protected:
	UPROPERTY()
	AMechBaseCharacter* OwningCharacter;
	
	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent; 
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;
};
