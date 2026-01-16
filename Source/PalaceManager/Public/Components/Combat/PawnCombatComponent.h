// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"
#include "GameplayTagContainer.h"
#include "PawnCombatComponent.generated.h"

class AMechWeaponBase;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	LeftHand,
	RightHand,
	Mouth
};
/**
 * 
 */
UCLASS()
class PALACEMANAGER_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category= "Mech|Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AMechWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon = false);

	UFUNCTION(BlueprintCallable, Category= "Mech|Combat")
	AMechWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;
	
	UPROPERTY(BlueprintReadWrite, Category= "Mech|Combat")
	FGameplayTag CurrentEquippedWeaponTag;
	
	UFUNCTION(BlueprintCallable, Category= "Mech|Combat")
	AMechWeaponBase* GetCharacterCurrentEquippedWeapon() const;
	
	UFUNCTION(BlueprintCallable, Category= "Mech|Combat")
	void ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType = EToggleDamageType::CurrentEquippedWeapon);
	
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	FGameplayTag CurrentLeftHandTag;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	FGameplayTag CurrentRightHandTag;
	
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	FGameplayTag CurrentMouseHandTag;
	
	virtual void OnHitTargetActor(AActor* HitActor);
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor);
	
protected:
	virtual void ToggleCurrentEquippedWeaponCollision(bool bShouldEnable);
	virtual void ToggleBodyCollsionBoxCollision(bool bShouldEnable,EToggleDamageType ToggleDamageType);
	
	TArray<AActor*> OverlappedActors;
	
private:
	TMap<FGameplayTag, AMechWeaponBase*> CharacterCarriedWeaponMap;
};
