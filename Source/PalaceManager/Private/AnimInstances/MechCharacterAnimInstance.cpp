// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/MechCharacterAnimInstance.h"
#include "Characters/MechBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMechCharacterAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<AMechBaseCharacter>(TryGetPawnOwner());
	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UMechCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!OwningCharacter || !OwningMovementComponent)
	{
		return;
	}
	
	GroundSpeed = OwningCharacter->GetVelocity().Size2D();
	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D()>0.f;
}
