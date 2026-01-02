// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"
#include "AbilitySystem/MechAttributeSet.h"
#include "MechGameplayTags.h"

struct FMechDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)

	FMechDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMechAttributeSet,AttackPower,Source,false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMechAttributeSet,DefensePower,Target,false)
	}
};

static const FMechDamageCapture& GetMechDamageCapture()
{
	static FMechDamageCapture MechDamageCapture;
	return MechDamageCapture;
}

UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{	
	/*Slow way of doing capture*/
	//FProperty* AttackPowerProperty = FindFieldChecked<FProperty>(
	//	UWarriorAttributeSet::StaticClass(),
	//	GET_MEMBER_NAME_CHECKED(UWarriorAttributeSet,AttackPower)
	//);

	//FGameplayEffectAttributeCaptureDefinition AttackPowerCaptureDefinition(
	//	AttackPowerProperty,
	//	EGameplayEffectAttributeCaptureSource::Source,
	//	false
	//);

	//RelevantAttributesToCapture.Add(AttackPowerCaptureDefinition);

	RelevantAttributesToCapture.Add(GetMechDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetMechDamageCapture().DefensePowerDef);
}
