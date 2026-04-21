// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/MechEnemyGameplayAbility.h"
#include "Characters/MechEnemyCharacter.h"
#include "AbilitySystem/MechAbilitySystemComponent.h"
#include "MechGameplayTags.h"

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

FGameplayEffectSpecHandle UMechEnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(
	TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat)
{
	check(EffectClass);

	FGameplayEffectContextHandle ContextHandle = GetMechAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(),GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle EffectSpecHandle = GetMechAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		MechGameplayTags::Shared_SetByCaller_BaseDamage,
		InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel())
	);

	return EffectSpecHandle;
}
