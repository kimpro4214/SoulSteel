// Fill out your copyright notice in the Description page of Project Settings.


#include "MechFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/MechAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "MechGameplayTags.h"

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
	
	if (ASC->HasMatchingGameplayTag(TagToRemove))
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

bool UMechFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
	check(QueryPawn && TargetPawn);

	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (QueryTeamAgent && TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}

	return false;
}

float UMechFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel)
{
	return InScalableFloat.GetValueAtLevel(InLevel);
}

FGameplayTag UMechFunctionLibrary::ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim,
	float& OutAngleDifference)
{
	check(InAttacker && InVictim);

	const FVector VictimForward = InVictim->GetActorForwardVector();
	const FVector VictimToAttackerNormalized = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();

	const float DotResult = FVector::DotProduct(VictimForward,VictimToAttackerNormalized);
	OutAngleDifference = UKismetMathLibrary::DegAcos(DotResult);

	const FVector CrossResult = FVector::CrossProduct(VictimForward,VictimToAttackerNormalized);

	if (CrossResult.Z < 0.f)
	{
		OutAngleDifference *= -1.f;
	}

	if (OutAngleDifference>=-45.f && OutAngleDifference <=45.f)
	{
		return MechGameplayTags::Shared_Status_HitReact_Front;
	}
	else if (OutAngleDifference<-45.f && OutAngleDifference>=-135.f)
	{
		return MechGameplayTags::Shared_Status_HitReact_Left;
	}
	else if (OutAngleDifference<-135.f || OutAngleDifference>135.f)
	{
		return MechGameplayTags::Shared_Status_HitReact_Back;
	}
	else if(OutAngleDifference>45.f && OutAngleDifference<=135.f)
	{
		return MechGameplayTags::Shared_Status_HitReact_Right;
	}

	return MechGameplayTags::Shared_Status_HitReact_Front;
}
