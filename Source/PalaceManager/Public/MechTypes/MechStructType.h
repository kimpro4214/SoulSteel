// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h" 
#include "ScalableFloat.h"        
#include "MechStructType.generated.h"

class UMechHeroLinkedAnimLayer;     
class UMechHeroGameplayAbility;     
class UInputMappingContext;         

// --- ENUM 정의 ---

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


UENUM()
enum class EMechSuccessType : uint8
{
	Successful,
	Failed
};

// --- STRUCT 정의 ---

/**
 * 어빌리티 세트 구조체
 * (무기 데이터 구조체에서 이 구조체를 배열로 쓰기 때문에, 더 위에 정의되어야 합니다)
 */
USTRUCT(BlueprintType)
struct FMechHeroAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UMechHeroGameplayAbility> AbilityToGrant; 

	bool IsValid() const; 
};

/**
 * 무기 데이터 구조체
 */
USTRUCT(BlueprintType)
struct FMechHeroWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UMechHeroLinkedAnimLayer> WeaponAnimLayerToLink; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* WeaponInputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FMechHeroAbilitySet> DefaultWeaponAbilities; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat WeaponBaseDamage;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> SoftWeaponIconTexture;
};

UCLASS()
class PALACEMANAGER_API UMechStructType : public UObject
{
	GENERATED_BODY()
    
};