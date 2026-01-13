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
	if (MechAbilitySystemComponent)
	{
		if (const UMechAttributeSet* AttributeSet = MechAbilitySystemComponent->GetSet<UMechAttributeSet>())
		{
			UMechAttributeSet* MutableAS = const_cast<UMechAttributeSet*>(AttributeSet);
            
			MutableAS->SetCurrentHealth(MutableAS->GetMaxHealth());
		}
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
