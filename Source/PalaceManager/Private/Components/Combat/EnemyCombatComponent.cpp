// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "MechGameplayTags.h"
#include "Characters/MechEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "MechDebugHelper.h"

void UEnemyCombatComponent::OnRegister()
{
	Super::OnRegister();
	
	CurrentRightHandTag = MechGameplayTags::Enemy_WeaponSecondary;
	CurrentLeftHandTag = MechGameplayTags::Enemy_Weapon;
	CurrentMouseHandTag = MechGameplayTags::Enemy_WeaponMouth;
	
	AMechEnemyCharacter* OwningEnemyCharacter = GetOwningPawn<AMechEnemyCharacter>();
	if (OwningEnemyCharacter)
	{
		if (UBoxComponent* LeftHandBox = OwningEnemyCharacter->GetLeftHandCollisionBox())
		{
			LeftHandBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnHandCollisionBoxBeginOverlap);
		}

		if (UBoxComponent* RightHandBox = OwningEnemyCharacter->GetRightHandCollisionBox())
		{
			RightHandBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnHandCollisionBoxBeginOverlap);
		}
	}
}

void UEnemyCombatComponent::ToggleBodyCollsionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	AMechEnemyCharacter* OwningEnemyCharacter = GetOwningPawn<AMechEnemyCharacter>();

	check(OwningEnemyCharacter);

	UBoxComponent* LeftHandCollisionBox = OwningEnemyCharacter->GetLeftHandCollisionBox();
	UBoxComponent* RightHandCollisionBox = OwningEnemyCharacter->GetRightHandCollisionBox();

	check(LeftHandCollisionBox && RightHandCollisionBox);

	switch (ToggleDamageType)
	{
	case EToggleDamageType::LeftHand:
		LeftHandCollisionBox->SetCollisionEnabled(bShouldEnable? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;

	case EToggleDamageType::RightHand:
		RightHandCollisionBox->SetCollisionEnabled(bShouldEnable? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;

	default:
		break;
	}

	if (!bShouldEnable)
	{
		OverlappedActors.Empty();
	}
}

void UEnemyCombatComponent::OnHandCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetOwningPawn())
	{
		OnHitTargetActor(OtherActor);
	}
}

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}

	OverlappedActors.AddUnique(HitActor);

	//TODO:: Implement block check
	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = false;
	const bool bIsMyAttackUnblockable = false;

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		//TODO::check if the block is valid
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;

	if (bIsValidBlock)
	{
		//TODO::Handle successful block
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			MechGameplayTags::Shared_Event_MeleeHit,
			EventData
		);
	}
}
