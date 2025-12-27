// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MechAbilitySystemComponent.h"

void UMechAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid())
	{
		return;
	}
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag)) continue;
		
		TryActivateAbility(AbilitySpec.Handle );
	}
}

void UMechAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
}
