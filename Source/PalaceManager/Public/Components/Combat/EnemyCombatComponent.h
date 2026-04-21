// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "EnemyCombatComponent.generated.h"

/**
 * 
 */
UCLASS()
class PALACEMANAGER_API UEnemyCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()
	
protected:
	virtual void OnRegister() override;
	virtual void ToggleBodyCollsionBoxCollision(bool bShouldEnable,EToggleDamageType ToggleDamageType) override;
	
	UFUNCTION()
	void OnHandCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
	virtual void OnHitTargetActor(AActor* HitActor) override;
};
