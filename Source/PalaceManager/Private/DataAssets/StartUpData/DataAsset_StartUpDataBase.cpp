// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/MechAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/MechGameplayAbility.h"

void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UMechAbilitySystemComponent* InMechASCToGive,
	int32 ApplyLevel)
{
	check(InMechASCToGive);
	
	GrantAbilities(ActivateOnGivenAbilities, InMechASCToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilities, InMechASCToGive, ApplyLevel);

}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UMechGameplayAbility>>& InAbilitiesToGive,
	UMechAbilitySystemComponent* InMechASCToGive, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty())
	{
		return;
	}
	
	for (const TSubclassOf<UMechGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if (!Ability) continue;
		
		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InMechASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		
		InMechASCToGive->GiveAbility(AbilitySpec);
	}
}
