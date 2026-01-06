// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MechAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class PALACEMANAGER_API UMechAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);	
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);	

	UFUNCTION(BlueprintCallable, Category = "Mech|Ability")
	bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);
};
