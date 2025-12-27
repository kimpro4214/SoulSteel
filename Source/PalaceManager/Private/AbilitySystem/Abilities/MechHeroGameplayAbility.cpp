// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/MechHeroGameplayAbility.h"
#include "Characters/MechHeroCharacter.h"
#include "Controllers/MechHeroController.h"

AMechHeroCharacter* UMechHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
	if (!CachedMechHeroCharacter.IsValid())
	{
		CachedMechHeroCharacter = Cast<AMechHeroCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedMechHeroCharacter.IsValid() ? CachedMechHeroCharacter.Get() : nullptr;
}

AMechHeroController* UMechHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
	if (!CachedMechHeroController.IsValid())
	{
		CachedMechHeroController = Cast<AMechHeroController>(CurrentActorInfo->PlayerController);
	}
	return CachedMechHeroController.IsValid() ? CachedMechHeroController.Get() : nullptr;
}

UHeroCombatComponent* UMechHeroGameplayAbility::GetMechHeroCombatComponentActorInfo()
{
	return GetHeroCharacterFromActorInfo()->GetHeroCombatComponent();
}
