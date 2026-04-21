// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MechBaseCharacter.h"
#include "AbilitySystem/MechAbilitySystemComponent.h"
#include "AbilitySystem/MechAttributeSet.h"
#include "MotionWarpingComponent.h"

// Sets default values
AMechBaseCharacter::AMechBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	GetMesh()->bReceivesDecals = false;
	
	MechAbilitySystemComponent = CreateDefaultSubobject<UMechAbilitySystemComponent>(TEXT("MechAbilitySystemComponent"));
	
	MechAttributeSet = CreateDefaultSubobject<UMechAttributeSet>(TEXT("MechAttributeSet"));
	
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

UAbilitySystemComponent* AMechBaseCharacter::GetAbilitySystemComponent() const
{
	return GetMechAbilitySystemComponent();
}

UPawnCombatComponent* AMechBaseCharacter::GetPawnCombatComponent() const
{
	return nullptr;
}

UPawnUIComponent* AMechBaseCharacter::GetPawnUIComponent() const
{
	return nullptr;
}

void AMechBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (MechAbilitySystemComponent)
	{
		MechAbilitySystemComponent->InitAbilityActorInfo(this,this);
		
 		ensureMsgf(!CharacterStartupData.IsNull(), TEXT("forgot to assign start up data to %s"), *GetName());
	}
}



