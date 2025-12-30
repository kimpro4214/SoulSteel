// Fill out your copyright notice in the Description page of Project Settings.


#include "MechFunctionLibrary.h"
#include "Interfaces/PawnCombatInterface.h"

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
