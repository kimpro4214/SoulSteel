// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MechWidgetBase.h"
#include "Interfaces/PawnUIInterface.h"


void UMechWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
	{
		if (UHeroUIComponent* HeroUIComponent = PawnUIInterface->GetHeroUIComponent())
		{
			BP_OnOwningHeroUIComponentInitialized(HeroUIComponent);
		}
	}
}

void UMechWidgetBase::InitHeroWidget(UHeroUIComponent* HeroUIComponent)
{
	if (HeroUIComponent)
	{
		BP_OnOwningHeroUIComponentInitialized(HeroUIComponent);
	}
}

void UMechWidgetBase::InitEnemyCreatedWidget(AActor* OwningEnemyActor)
{
	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(OwningEnemyActor))
	{
		UEnemyUIComponent* EnemyUIComponent = PawnUIInterface->GetEnemyUIComponent();

		checkf(EnemyUIComponent,TEXT("Failed to extrac an EnemyUIComponent from %s"),*OwningEnemyActor->GetActorNameOrLabel());

		BP_OnOwningEnemyUIComponentInitialized(EnemyUIComponent);
	}
}
