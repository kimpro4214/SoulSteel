 // Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PawnCombatComponent.h"
#include "Items/Weapons/MechWeaponBase.h"
#include "Components/BoxComponent.h"

#include "MechDebugHelper.h"

 void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister,
	 AMechWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
 {
 	checkf(!CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister), TEXT("A named named %s has already been added as carried weapon"), *InWeaponTagToRegister.ToString());
	check(InWeaponToRegister);
 	
 	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);
 	
 	InWeaponToRegister->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
 	InWeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);
 	
 	if (bRegisterAsEquippedWeapon)
 	{
 		CurrentEquippedWeaponTag = InWeaponTagToRegister;
 	}
 	
 	const FString WeaponString = FString::Printf(TEXT("A weapon named: %s has been registered using the tag %s"), *InWeaponToRegister->GetName(), *InWeaponTagToRegister.ToString());
 	Debug::Print(WeaponString);
 }

 AMechWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
 {
 	if (CharacterCarriedWeaponMap.Contains(InWeaponTagToGet))
 	{
 		if (AMechWeaponBase* const* FoundWeapon = CharacterCarriedWeaponMap.Find(InWeaponTagToGet))
 		{
 			return *FoundWeapon;
 		}
 	}
 	return nullptr;
 }

 AMechWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon() const
 {
 	if (!CurrentEquippedWeaponTag.IsValid())
 	{
 		return nullptr;
 	}
 	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
 }

 void UPawnCombatComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
 {
 	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
 	{
 		AMechWeaponBase* WeaponToToggle = GetCharacterCurrentEquippedWeapon();
 		
 		check(WeaponToToggle);
 		
 		if (bShouldEnable)
 		{
 			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
 		}
	    else
	    {
	    	WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	    }
 	}
 	//todo : Handle body collision boxes
 }

 void UPawnCombatComponent::OnHitTargetActor(AActor* HitActor)
 {
 }

 void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
 {
 }
