// Fill out your copyright notice in the Description page of Project Settings.


#include "MechFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/MechAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"

UMechAbilitySystemComponent* UMechFunctionLibrary::NativeGetMechASCFromActor(AActor* InActor)
{
	check(InActor);
	return CastChecked<UMechAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UMechFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UMechAbilitySystemComponent* ASC = NativeGetMechASCFromActor(InActor);
	if (!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
	
}

void UMechFunctionLibrary::RemoveGameplayFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UMechAbilitySystemComponent* ASC = NativeGetMechASCFromActor(InActor);
	
	if (!ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}

}

bool UMechFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UMechAbilitySystemComponent* ASC = NativeGetMechASCFromActor(InActor);

	return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UMechFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck,
	EMechConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck)? EMechConfirmType::Yes : EMechConfirmType::No;
}

UPawnCombatComponent* UMechFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	check(InActor);
	if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}
	return nullptr;
}

UPawnCombatComponent* UMechFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor,
	EMechValidType& OutValidType)
{
	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);
	
	OutValidType = CombatComponent? EMechValidType::Valid : EMechValidType::Invalid;
	
	return CombatComponent;
}
