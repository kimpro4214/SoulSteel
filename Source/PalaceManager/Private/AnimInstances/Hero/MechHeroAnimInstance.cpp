// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Hero/MechHeroAnimInstance.h"
#include "Characters/MechHeroCharacter.h"

void UMechHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	if (OwningCharacter)
	{
		OwningHeroCharacter = Cast<AMechHeroCharacter>(OwningCharacter);
	}
}

void UMechHeroAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	
	if (bHasAcceleration)
	{
		IdleElpasedTime = 0.f;
		bShouldEnterRelaxState = false;
	}
	else
	{
		IdleElpasedTime += DeltaSeconds;
		
		bShouldEnterRelaxState = (IdleElpasedTime >= EnterRelaxStateThreshold);
	}
}
