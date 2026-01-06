// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MechGameplayAbility.h"
#include "MechHeroGameplayAbility.generated.h"

class AMechHeroCharacter;
class AMechHeroController;
class UHeroCombatComponent;

/**
 * 
 */
UCLASS()
class PALACEMANAGER_API UMechHeroGameplayAbility : public UMechGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Mech|Ability")
	AMechHeroCharacter* GetHeroCharacterFromActorInfo();
	
	UFUNCTION(BlueprintPure, Category = "Mech|Ability")
	AMechHeroController* GetHeroControllerFromActorInfo();
	
	UFUNCTION(BlueprintPure, Category = "Mech|Ability")
	UHeroCombatComponent* GetMechHeroCombatComponentActorInfo();
	
	UFUNCTION(BlueprintPure, Category = "Mech|Ability")
	FGameplayEffectSpecHandle MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass,float InWeaponBaseDamage,FGameplayTag InCurrentAttackTypeTag,int32 InCurrentComboCount);
private:
	TWeakObjectPtr<AMechHeroCharacter> CachedMechHeroCharacter;
	TWeakObjectPtr<AMechHeroController> CachedMechHeroController;
};
