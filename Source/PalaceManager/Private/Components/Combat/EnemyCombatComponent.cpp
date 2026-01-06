// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/EnemyCombatComponent.h"
#include "MechGameplayTags.h"
#include "MechDebugHelper.h"

void UEnemyCombatComponent::OnRegister()
{
	Super::OnRegister();
	
	CurrentRightHandTag = MechGameplayTags::Enemy_Weapon;
	CurrentLeftHandTag = MechGameplayTags::Enemy_WeaponSecondary;
}

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (HitActor)
	{
		Debug::Print(GetOwningPawn()->GetActorNameOrLabel() + TEXT(" is hitting ") + HitActor->GetActorNameOrLabel());
	}
}
