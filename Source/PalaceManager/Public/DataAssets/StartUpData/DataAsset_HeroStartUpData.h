#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "GameplayTagContainer.h"
#include "MechTypes/MechStructType.h" 
#include "DataAsset_HeroStartUpData.generated.h"


/**
 * */
UCLASS()
class PALACEMANAGER_API UDataAsset_HeroStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()
public:
	virtual void GiveToAbilitySystemComponent(UMechAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1) override;
    
private:
	// 이제 컴파일러는 "MechStructType.h"를 보고 FMechHeroAbilitySet이 뭔지 알 수 있습니다.
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData", meta=(TitleProperty = "InputTag"))
	TArray<FMechHeroAbilitySet> HeroStartAbilitySets;
};