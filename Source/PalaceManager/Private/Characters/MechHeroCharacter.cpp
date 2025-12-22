// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MechHeroCharacter.h"

#include "MechDebugHelper.h"

void AMechHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Debug::Print(TEXT("WORKING"));
}