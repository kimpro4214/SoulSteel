// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MechEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "Engine/AssetManager.h"
#include "Components/UI/EnemyUIComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/MechWidgetBase.h"
#include "Components/BoxComponent.h"
#include "MechFunctionLibrary.h"
#include "AbilitySystem/MechAttributeSet.h"
#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"

#include "MechDebugHelper.h"
AMechEnemyCharacter::AMechEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 180.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;
	
	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>("EnemyCombatComponent");
	
	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>("EnemyUIComponent");
	
	EnemyHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("EnemyHealthWidgetComponent");
	EnemyHealthWidgetComponent->SetupAttachment(GetMesh());
	
	LeftHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("LeftHandCollisionBox");
	LeftHandCollisionBox->SetupAttachment(GetMesh());
	LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnBodyCollisionBoxBeginOverlap);

	RightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("RightHandCollisionBox");
	RightHandCollisionBox->SetupAttachment(GetMesh());
	RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnBodyCollisionBoxBeginOverlap);
}

void AMechEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	 
	InitEnemyStartUpData();
}

#if WITH_EDITOR
void AMechEnemyCharacter::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass,LeftHandCollisionBoxAttachBoneName))
	{
		LeftHandCollisionBox->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,LeftHandCollisionBoxAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass,RightHandCollisionBoxAttachBoneName))
	{
		RightHandCollisionBox->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,RightHandCollisionBoxAttachBoneName);
	}
}
#endif
 
void AMechEnemyCharacter::OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (UMechFunctionLibrary::IsTargetPawnHostile(this,HitPawn))
		{
			EnemyCombatComponent->OnHitTargetActor(HitPawn);
		}
	}
}

void AMechEnemyCharacter::InitEnemyStartUpData()
{
	if (CharacterStartupData.IsNull())
	{
		return;
	}	
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartupData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this]()
			{
				if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartupData.Get())
				{
					LoadedData->GiveToAbilitySystemComponent(MechAbilitySystemComponent);
				}
			}
		)
	);
}

UPawnCombatComponent* AMechEnemyCharacter::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}

UPawnUIComponent* AMechEnemyCharacter::GetPawnUIComponent() const
{
	return EnemyUIComponent;
}

UEnemyUIComponent* AMechEnemyCharacter::GetEnemyUIComponent() const
{
	return EnemyUIComponent;
}

void AMechEnemyCharacter::InitBossFight()
 {
	if (!bIsBoss) return;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
    
	// BossUIAbilityClass가 설정되어 있는지 확인
	if (ASC && BossUIAbilityClass)
	{
		// 1. Spec 생성 (내 커스텀 클래스 기반)
		FGameplayAbilitySpec Spec(BossUIAbilityClass, 1, INDEX_NONE, this);
        
		// 2. 어빌리티 부여 및 즉시 실행
		// (GiveAbilityAndActivateOnce는 부모인 UGameplayAbilitySpec을 받으므로 호환됨)
		ASC->GiveAbilityAndActivateOnce(Spec);
	}
}

void AMechEnemyCharacter::ResetBossState()
{
	if (!bIsBoss) return;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (ASC)
	{
		float MaxHP = ASC->GetNumericAttribute(UMechAttributeSet::GetMaxHealthAttribute());
		ASC->SetNumericAttributeBase(UMechAttributeSet::GetCurrentHealthAttribute(), MaxHP);
	}

	if (EnemyUIComponent)
	{
		EnemyUIComponent->RemoveEnemyDrawnWidgetsIfAny();
	}

	SetActorLocation(StartingLocation);
	SetActorRotation(StartingRotation);
}

void AMechEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	StartingLocation = GetActorLocation();
	StartingRotation = GetActorRotation();
	
	if (UMechWidgetBase* HealthWidget = Cast<UMechWidgetBase>(EnemyHealthWidgetComponent->GetUserWidgetObject()))
	{
		HealthWidget->InitEnemyCreatedWidget(this);
	}
}
