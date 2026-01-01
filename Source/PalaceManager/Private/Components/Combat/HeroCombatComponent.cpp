// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/HeroCombatComponent.h"

#include "MechDebugHelper.h"

void UHeroCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	Debug::Print(GetOwningPawn()->GetActorNameOrLabel() + TEXT("hit") + HitActor->GetActorNameOrLabel(),FColor::Green);
	Super::OnHitTargetActor(HitActor);
}

void UHeroCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	Debug::Print(GetOwningPawn()->GetActorNameOrLabel() + TEXT("s weapon pulled from ") + InteractedActor->GetActorNameOrLabel(),FColor::Red);
	Super::OnWeaponPulledFromTargetActor(InteractedActor);
}
