// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/MechEnemyGameplayAbility.h"
#include "Characters/MechEnemyCharacter.h"

AMechEnemyCharacter* UMechEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
	if (!CachedMechEnemyCharacter.IsValid())
	{
		CachedMechEnemyCharacter = Cast<AMechEnemyCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedMechEnemyCharacter.IsValid() ? CachedMechEnemyCharacter.Get() : nullptr;
}

UEnemyCombatComponent* UMechEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
	return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}
