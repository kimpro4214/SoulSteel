// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MechFunctionLibrary.generated.h"

UENUM()
enum class EMechConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class EMechValidType : uint8
{
	Valid,
	Invalid
};

class UMechAbilitySystemComponent;
class UPawnCombatComponent;
/**
 * 
 */
UCLASS()
class PALACEMANAGER_API UMechFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static UMechAbilitySystemComponent* NativeGetMechASCFromActor(AActor* InActor);
	
	UFUNCTION(BlueprintCallable, Category="Mech|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);
	
	UFUNCTION(BlueprintCallable, Category="Mech|FunctionLibrary")
	static void RemoveGameplayFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);
	
	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);
	
	UFUNCTION(BlueprintCallable, Category="Mech|FunctionLibrary",meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EMechConfirmType& OutConfirmType);
	
	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);
	
	UFUNCTION(BlueprintCallable, Category= "Mech|FunctionLibrary", meta = (DisplayName = "Get Pawn Combat Component From Actor", ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor, EMechValidType& OutValidType);
};
