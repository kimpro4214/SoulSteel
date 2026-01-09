// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/HeroCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "MechGameplayTags.h"

#include "MechDebugHelper.h"
#include "Items/Weapons/MechHeroWeapon.h"

void UHeroCombatComponent::OnRegister()
{
	Super::OnRegister();
	
	CurrentRightHandTag = MechGameplayTags::Player_Weapon_Punch;
	CurrentLeftHandTag = MechGameplayTags::Player_Weapon_PunchSecondary;
}

AMechHeroWeapon* UHeroCombatComponent::GetHeroCurrentEquippedWeapon() const
{	
	return Cast<AMechHeroWeapon>(GetCharacterCurrentEquippedWeapon());
}

float UHeroCombatComponent::GetHeroCurrentEquippWeaponDamageAtLevel(float InLevel) const
{
	return GetHeroCurrentEquippedWeapon()->HeroWeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel);
}

void UHeroCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}

	OverlappedActors.AddUnique(HitActor);

	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		MechGameplayTags::Shared_Event_MeleeHit,
		Data
	);
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		MechGameplayTags::Player_Event_HitPause,
		FGameplayEventData()
	);
}

void UHeroCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		MechGameplayTags::Player_Event_HitPause, 
		FGameplayEventData()
	);
}


