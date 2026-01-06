// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/MechWeaponBase.h"
#include "MechTypes/MechStructType.h"
#include "MechHeroWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PALACEMANAGER_API AMechHeroWeapon : public AMechWeaponBase
{
	GENERATED_BODY()
public:
	AMechHeroWeapon();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FMechHeroWeaponData HeroWeaponData;
	
};
