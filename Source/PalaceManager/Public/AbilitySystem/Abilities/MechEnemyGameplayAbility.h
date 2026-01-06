// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MechGameplayAbility.h"
#include "MechEnemyGameplayAbility.generated.h"

class AMechEnemyCharacter;
class UEnemyCombatComponent;
/**
 * 
 */
UCLASS()
class PALACEMANAGER_API UMechEnemyGameplayAbility : public UMechGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Mech|Ability")
	AMechEnemyCharacter* GetEnemyCharacterFromActorInfo();
	
	UFUNCTION(BlueprintPure, Category = "Mech|Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();
	
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass,const FScalableFloat& InDamageScalableFloat);
	
private:
	TWeakObjectPtr<AMechEnemyCharacter> CachedMechEnemyCharacter;
};
