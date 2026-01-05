// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/MechAIController.h"
#include "Navigation/CrowdFollowingComponent.h"

#include "MechDebugHelper.h"
AMechAIController::AMechAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	{
		if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
		{
			Debug::Print(TEXT("CrowdFollowingComponent valid"),FColor::Green);
		}
	}
}
