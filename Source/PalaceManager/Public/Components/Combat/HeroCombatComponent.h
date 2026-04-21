// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "HeroCombatComponent.generated.h"

/**
 * 
 */
UCLASS()
class PALACEMANAGER_API UHeroCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

protected:
	virtual void OnRegister() override;
public:
	
	UFUNCTION(BlueprintCallable, Category = "Mech|Combat")
	AMechHeroWeapon* GetHeroCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Mech|Combat")
	float GetHeroCurrentEquippWeaponDamageAtLevel(float InLevel) const;
	
	virtual void OnHitTargetActor(AActor* HitActor) override;
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor) override;
};
