// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/MechHeroController.h"

AMechHeroController::AMechHeroController()
{
	HeroTeamID = FGenericTeamId(0);
}

FGenericTeamId AMechHeroController::GetGenericTeamId() const
{
	return HeroTeamID;
}
