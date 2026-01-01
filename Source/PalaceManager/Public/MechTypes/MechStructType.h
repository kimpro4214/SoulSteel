// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h" // FGameplayTag를 쓰기 위해 필요
#include "ScalableFloat.h"        // FScalableFloat를 쓰기 위해 필요
#include "MechStructType.generated.h"

// --- 전방 선언 (Forward Declarations) ---
// 아직 헤더를 include 하지 않아도 클래스가 있다고 알려주는 역할입니다.
// 강의의 WarriorHero... 클래스들을 본인의 Mech 클래스 이름으로 맞춰야 합니다.
class UMechHeroLinkedAnimLayer;     // (가정) 애니메이션 레이어 클래스
class UMechHeroGameplayAbility;     // (가정) 어빌리티 클래스
class UInputMappingContext;         // 입력 매핑 컨텍스트

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
	TSubclassOf<UMechHeroGameplayAbility> AbilityToGrant; // 본인의 어빌리티 클래스 이름으로 변경 필요

	bool IsValid() const; // .cpp 파일에서 구현 필요
};

/**
 * 무기 데이터 구조체
 */
USTRUCT(BlueprintType)
struct FMechHeroWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UMechHeroLinkedAnimLayer> WeaponAnimLayerToLink; // 본인의 AnimLayer 클래스 이름으로 변경 필요

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* WeaponInputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FMechHeroAbilitySet> DefaultWeaponAbilities; // 위에서 만든 구조체를 배열로 사용

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat WeaponBaseDamage;
};

UCLASS()
class PALACEMANAGER_API UMechStructType : public UObject
{
	GENERATED_BODY()
    
};