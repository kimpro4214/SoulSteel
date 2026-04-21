// Copyright Epic Games, Inc. All Rights Reserved.

#include "PalaceManagerGameMode.h"
#include "UObject/ConstructorHelpers.h"

// 필요하다면 Lyra PlayerController 헤더 추가
// #include "Core/LyraPlayerController.h"

APalaceManagerGameMode::APalaceManagerGameMode()
{
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
        TEXT("/Game/Characters/Heroes/B_Hero_Default.B_Hero_Default_C")
    );
    if (PlayerPawnBPClass.Class != nullptr)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
        UE_LOG(LogTemp, Warning, TEXT("Default Pawn set to B_Hero_Default"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load B_Hero_Default!"));
    }

}
