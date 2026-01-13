// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MechHeroCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/MechInputComponent.h"
#include "MechGameplayTags.h"
#include "DataAssets/StartUpData/DataAsset_HeroStartUpData.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "AbilitySystem/MechAbilitySystemComponent.h"
#include "AbilitySystem/MechAttributeSet.h"
#include "Components/UI/HeroUIComponent.h"

#include "MechDebugHelper.h"
#include "DynamicMesh/DynamicMesh3.h"

AMechHeroCharacter::AMechHeroCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
	HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponent"));
	HeroUIComponent = CreateDefaultSubobject<UHeroUIComponent>(TEXT("HeroUIComponent"));
}

UPawnCombatComponent* AMechHeroCharacter::GetPawnCombatComponent() const
{
	return HeroCombatComponent;
}

UPawnUIComponent* AMechHeroCharacter::GetPawnUIComponent() const
{
	return HeroUIComponent;
}

UHeroUIComponent* AMechHeroCharacter::GetHeroUIComponent() const
{
	return Super::GetHeroUIComponent();
}

void AMechHeroCharacter::HandleDeath()
{
	BP_OnDeath();
}

void AMechHeroCharacter::FullHeal()
{
    // 1. 체력 복구 (기존 로직 유지)
    if (MechAbilitySystemComponent)
    {
       // const_cast는 위험할 수 있으니, 가능하면 AttributeSet 내부에 InitHealth() 같은 함수를 만드는 것을 권장합니다.
       // 일단 현재 로직대로 유지합니다.
       if (const UMechAttributeSet* AttributeSet = MechAbilitySystemComponent->GetSet<UMechAttributeSet>())
       {
          UMechAttributeSet* MutableAS = const_cast<UMechAttributeSet*>(AttributeSet);
          MutableAS->SetCurrentHealth(MutableAS->GetMaxHealth());
       }

        // 2. [중요] 실행 중인 모든 어빌리티 취소 (공격/구르기 불가 해결)
        // 죽음 어빌리티가 아직 Active 상태라면 다른 어빌리티(BlockAbilities 태그 때문)가 실행되지 않습니다.
        // 이를 강제로 종료시킵니다.
        MechAbilitySystemComponent->CancelAllAbilities();
        
        // 3. 태그 제거 (기존 로직 + GE로 적용된 태그도 제거)
        // RemoveLooseGameplayTag는 수동으로 추가한 태그만 지웁니다.
        // 만약 GameplayEffect로 인해 Dead 태그가 붙어있다면 아래 코드가 추가로 필요할 수 있습니다.
        FGameplayTagContainer DeadTags;
        DeadTags.AddTag(MechGameplayTags::Shared_Status_Dead);
        
        MechAbilitySystemComponent->RemoveActiveEffectsWithTags(DeadTags); // GE 제거
        MechAbilitySystemComponent->RemoveLooseGameplayTag(MechGameplayTags::Shared_Status_Dead); // Loose 태그 제거
    }

    // 4. [중요] 애니메이션 몽타주 강제 정지 (캐릭터 굳음 해결)
    if (GetMesh() && GetMesh()->GetAnimInstance())
    {
        // 현재 재생 중인 사망 몽타주를 0.2초에 걸쳐 자연스럽게 끕니다.
        GetMesh()->GetAnimInstance()->StopAllMontages(0.2f);
    }

    // 5. [중요] 콜리전 및 움직임 복구 (AI 인식 불가 해결)
    // 사망 시 Ragdoll(Physics)이나 NoCollision으로 변했다면 다시 Pawn으로 돌려놔야 AI가 인식합니다.
    if (GetCapsuleComponent())
    {
        GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn")); // 혹은 프로젝트에 맞는 프리셋 이름
    }

    if (GetCharacterMovement())
    {
        // 움직임 모드를 다시 걷기로 변경
        GetCharacterMovement()->SetMovementMode(MOVE_Walking);
        // 혹시 미끄러지거나 남아있는 가속도 제거
        GetCharacterMovement()->StopMovementImmediately(); 
    }

    // 6. UI 업데이트 (기존 로직 유지)
    if (HeroUIComponent && MechAbilitySystemComponent)
    {
       const float NewHealth =
          MechAbilitySystemComponent->GetNumericAttribute(UMechAttributeSet::GetCurrentHealthAttribute());
       const float MaxHealth =
          MechAbilitySystemComponent->GetNumericAttribute(UMechAttributeSet::GetMaxHealthAttribute());

       const float NewPercent = (MaxHealth > 0.f) ? (NewHealth / MaxHealth) : 0.f;
       HeroUIComponent->BroadcastHealthPercent(NewPercent);
    }
}

void AMechHeroCharacter::ServerRevive_Implementation()
{
	// 1. [중요] GAS 어빌리티 시스템 정리
	if (MechAbilitySystemComponent)
	{
		// 죽음 어빌리티 강제 종료 (이걸 해야 'OnEndAbility'가 텅 빈 상태로 끝나서 상태가 초기화됨)
		MechAbilitySystemComponent->CancelAllAbilities();

		// 태그 제거
		FGameplayTagContainer DeadTags;
		DeadTags.AddTag(MechGameplayTags::Shared_Status_Dead);
		MechAbilitySystemComponent->RemoveActiveEffectsWithTags(DeadTags);
		MechAbilitySystemComponent->RemoveLooseGameplayTag(MechGameplayTags::Shared_Status_Dead);
	}

	// 2. [중요] 애니메이션 굳음 해결
	if (GetMesh())
	{
		// 블루프린트에서 '애니 일시정지'를 썼다면 이걸 false로 해줘야 다시 움직입니다.
		GetMesh()->bPauseAnims = false; 
        
		if (GetMesh()->GetAnimInstance())
		{
			// 몽타주 즉시 끄기 (Idle 상태로 복귀)
			GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
		}
	}

	// 3. 체력 100% 회복
	if (MechAbilitySystemComponent)
	{
		if (const UMechAttributeSet* AttributeSet = MechAbilitySystemComponent->GetSet<UMechAttributeSet>())
		{
			UMechAttributeSet* MutableAS = const_cast<UMechAttributeSet*>(AttributeSet);
			MutableAS->SetCurrentHealth(MutableAS->GetMaxHealth());
		}
	}

	// 4. 콜리전 복구 (AI가 다시 나를 보게 함)
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	}

	// 5. 움직임 모드 복구
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}

void AMechHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (!CharacterStartupData.IsNull())
	{
		if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartupData.LoadSynchronous())
		{
			LoadedData->GiveToAbilitySystemComponent(MechAbilitySystemComponent);
		}
	}
}

void AMechHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config"));
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	
	check(Subsystem);
	
	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext,0);
	
	UMechInputComponent* MechInputComponent = CastChecked<UMechInputComponent>(PlayerInputComponent);
	
	MechInputComponent->BindNativeInputAction(InputConfigDataAsset, MechGameplayTags::InputTag_Move,ETriggerEvent::Triggered,this,&ThisClass::Input_Move);
	MechInputComponent->BindNativeInputAction(InputConfigDataAsset, MechGameplayTags::InputTag_Look,ETriggerEvent::Triggered,this,&ThisClass::Input_Look);
	MechInputComponent->BindNativeInputAction(InputConfigDataAsset, MechGameplayTags::InputTag_Jump, ETriggerEvent::Started, this, &ThisClass::Input_Jump);
	
	MechInputComponent->BindAbilityInputAction(InputConfigDataAsset,this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}


void AMechHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMechHeroCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
	
	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		
		AddMovementInput(ForwardDirection, MovementVector.Y);
	}
	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMechHeroCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	
	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}
	
	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMechHeroCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	MechAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AMechHeroCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	MechAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}

void AMechHeroCharacter::Input_Jump(const FInputActionValue& InputActionValue)
{
	Jump();
}
