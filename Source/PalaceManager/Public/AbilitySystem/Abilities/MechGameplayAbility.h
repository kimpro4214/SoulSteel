// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MechTypes/MechStructType.h"
#include "MechGameplayAbility.generated.h"

class UPawnCombatComponent;
class UMechAbilitySystemComponent;

UENUM(BlueprintType)
enum class EMechAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};
/**
 * 
 */
UCLASS()
class PALACEMANAGER_API UMechGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
protected:
	//~ Begin UGameplayAbility Interface
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

	UPROPERTY(EditDefaultsOnly, Category= "MechAbility")
	EMechAbilityActivationPolicy AbilityActivationPolicy = EMechAbilityActivationPolicy::OnTriggered;
	
	UFUNCTION(BlueprintPure, Category = "Mech|Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;
	
	UFUNCTION(BlueprintPure, Category = "Mech|Ability")
	UMechAbilitySystemComponent* GetMechAbilitySystemComponentFromActorInfo() const;
	
	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor,const FGameplayEffectSpecHandle& InSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "Mech|Ability", meta = (DisplayName = "Apply Gameplay Effect Spec Handle To Target Actor", ExpandEnumAsExecs = "OutSuccessType"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor,const FGameplayEffectSpecHandle& InSpecHandle,EMechSuccessType& OutSuccessType);
};
