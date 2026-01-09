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
	
public:
	virtual void OnHitTargetActor(AActor* HitActor) override;
};
