// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"
#include "AbilitySystem/MechAttributeSet.h"
#include "MechGameplayTags.h"

#include "MechDebugHelper.h"

struct FMechDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)

	FMechDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMechAttributeSet,AttackPower,Source,false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMechAttributeSet,DefensePower,Target,false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMechAttributeSet,DamageTaken,Target,false)
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
	//	UMechAttributeSet::StaticClass(),
	//	GET_MEMBER_NAME_CHECKED(UMechAttributeSet,AttackPower)
	//);

	//FGameplayEffectAttributeCaptureDefinition AttackPowerCaptureDefinition(
	//	AttackPowerProperty,
	//	EGameplayEffectAttributeCaptureSource::Source,
	//	false
	//);

	//RelevantAttributesToCapture.Add(AttackPowerCaptureDefinition);

	RelevantAttributesToCapture.Add(GetMechDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetMechDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetMechDamageCapture().DamageTakenDef);
}

void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{	
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();
	
	/*EffectSpec.GetContext().GetSourceObject();
	EffectSpec.GetContext().GetAbility();
	EffectSpec.GetContext().GetInstigator();
	EffectSpec.GetContext().GetEffectCauser();*/

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetMechDamageCapture().AttackPowerDef,EvaluateParameters,SourceAttackPower);

	/*Debug::Print(TEXT("SourceAttackPower"),SourceAttackPower);*/
	float BaseDamage = 0.f;
	int32 UsedLightAttckComboCount = 0;
	int32 UsedHeavyAttackComboCount = 0;

	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(MechGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
			/*Debug::Print(TEXT("BaseDamage"),BaseDamage);*/
		}

		if (TagMagnitude.Key.MatchesTagExact(MechGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			UsedLightAttckComboCount = TagMagnitude.Value;
			/*Debug::Print(TEXT("UsedLightAttckComboCount"),UsedLightAttckComboCount);*/
		}

		if (TagMagnitude.Key.MatchesTagExact(MechGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			UsedHeavyAttackComboCount = TagMagnitude.Value;
			/*Debug::Print(TEXT("UsedHeavyAttackComboCount"),UsedHeavyAttackComboCount);*/
		}
	}

	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetMechDamageCapture().DefensePowerDef,EvaluateParameters,TargetDefensePower);
	/*Debug::Print(TEXT("TargetDefensePower"),TargetDefensePower);*/

	if (UsedLightAttckComboCount != 0)
	{
		const float DamageIncreasePercentLight = (UsedLightAttckComboCount - 1) * 0.05f + 1.f;

		BaseDamage *= DamageIncreasePercentLight;
		/*Debug::Print(TEXT("ScaledBaseDamageLight"),BaseDamage);*/
	}

	if (UsedHeavyAttackComboCount != 0)
	{
		const float DamageIncreasePercentHeavy = UsedHeavyAttackComboCount * 0.15f + 1.f;

		BaseDamage *= DamageIncreasePercentHeavy;
		/*Debug::Print(TEXT("ScaledBaseDamageHeavy"),BaseDamage);*/
	}

	const float FinalDamageDone = BaseDamage * SourceAttackPower / TargetDefensePower;
	/*
	Debug::Print(TEXT("FinalDamageDone"),FinalDamageDone);
	*/

	if (FinalDamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetMechDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamageDone
			)
		);
	}
}