// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/MechHeroGameplayAbility.h"
#include "Characters/MechHeroCharacter.h"
#include "Controllers/MechHeroController.h"
#include "AbilitySystem/MechAbilitySystemComponent.h"
#include "MechGameplayTags.h"

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

FGameplayEffectSpecHandle UMechHeroGameplayAbility::MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InCurrentComboCount)
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
		InWeaponBaseDamage
	);

	if (InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag,InCurrentComboCount);
	}

	return EffectSpecHandle;
}