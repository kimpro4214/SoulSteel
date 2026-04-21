# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**PalaceManager** is an Unreal Engine 5.4 action game built around the Gameplay Ability System (GAS). The codebase is a single C++ module (`PalaceManager`) with no scripting layer — all logic is in C++.

## Build Commands

Build via UnrealBuildTool from the UE5 engine install:

```bash
# Development Editor build (most common during iteration)
"<UE5_ROOT>/Engine/Build/BatchFiles/Build.bat" PalaceManagerEditor Win64 Development "C:/Users/qnfgh/PalaceManager/PalaceManager.uproject"

# Shipping build
"<UE5_ROOT>/Engine/Build/BatchFiles/Build.bat" PalaceManager Win64 Shipping "C:/Users/qnfgh/PalaceManager/PalaceManager.uproject"
```

Alternatively, open `PalaceManager.sln` in Visual Studio 2022 and build the `Development Editor` configuration. Live Coding is enabled via the RiderLink plugin.

There are no automated tests configured in this project.

## Architecture

### Character Hierarchy

`AMechBaseCharacter` (implements `IAbilitySystemInterface`, `IPawnCombatInterface`, `IPawnUIInterface`) is the root for all pawns. It owns `MechAbilitySystemComponent`, `MechAttributeSet`, and `MotionWarpingComponent`, and holds a `TSoftObjectPtr<UDataAsset_StartUpDataBase>` that drives ability/effect grants on possession.

- `AMechHeroCharacter` — player character, uses `MechHeroController` and `MechInputComponent`
- `AMechEnemyCharacter` — AI character, uses `MechAIController` with Behavior Tree

### Gameplay Ability System (GAS) Wiring

Abilities are not granted directly — they are registered through `DataAsset_StartUpDataBase::GiveToAbilitySystemComponent()`, which reads `ActivateOnGivenAbilities`, `ReactiveAbilities`, and `StartUpGameplayEffects` arrays from a DataAsset assigned per character class. Hero and enemy each have their own subclass (`DataAsset_HeroStartUpData`, `DataAsset_EnemyStartUpData`).

Ability subclasses:
- `UMechGameplayAbility` — base, adds `MechFunctionLibrary` helpers and `MotionWarping` access
- `UMechHeroGameplayAbility` — hero-specific (input-bound abilities)
- `UMechEnemyGameplayAbility` — enemy-specific

Damage flows through `GEExecCalc_DamageTaken`, which reads `AttackPower`, `DefensePower`, and `DamageTaken` from `MechAttributeSet`. Attribute changes broadcast to UI via the `IPawnUIInterface` cached in `MechAttributeSet`.

### Gameplay Tags

All tags are declared in `MechGameplayTags.h` (namespace `MechGameplayTags`) with `UE_DECLARE_GAMEPLAY_TAG_EXTERN` and defined in `MechGameplayTags.cpp`. Tag values are registered in `Config/DefaultGameplayTags.ini`. When adding a new tag, declare it in both files and add the ini entry.

Tag naming convention:
- `InputTag_*` — Enhanced Input action bindings
- `Player_Ability_*` / `Enemy_Ability_*` — ability identifiers
- `Player_Weapon_*` / `Enemy_Weapon_*` — weapon slot keys for `PawnCombatComponent`
- `Shared_*` — cross-pawn events and statuses (HitReact, Death, MeleeHit)

### Component Architecture

Two component hierarchies hang off every pawn:

**Combat:** `UPawnCombatComponent` → `UHeroCombatComponent` / `UEnemyCombatComponent`
- Maintains a `TMap<FGameplayTag, AMechWeaponBase*>` of carried weapons
- `ToggleWeaponCollision` enables/disables hit detection per animation notify
- `OnHitTargetActor` / `OnWeaponPulledFromTargetActor` handle overlap deduplication via `OverlappedActors`

**UI:** `UPawnUIComponent` → `UHeroUIComponent` / `UEnemyUIComponent`
- Exposes delegates that `MechAttributeSet::PostGameplayEffectExecute` fires when attributes change

Both hierarchies are accessed polymorphically through the `IPawnCombatInterface` / `IPawnUIInterface` interfaces. Use `MechFunctionLibrary` helpers rather than direct casts where possible.

### Input

`DataAsset_InputConfig` maps `UInputAction` assets to Gameplay Tags. `MechInputComponent` binds these at runtime using `BindNativeInputAction` / `BindAbilityInputAction`. Enhanced Input is the only supported input path.

### AI

Enemy AI uses Unreal's Behavior Tree with custom nodes:
- `BTService_OrientToTargetActor` — rotates enemy toward target each tick
- `BTTask_RotateToFaceTarget` — one-shot rotate task

The `MechAIController` owns the BT and Blackboard.

### Weapons

`AMechWeaponBase` (Actor) is the base weapon spawned and attached to sockets. `AMechHeroWeapon` extends it for player weapons. Weapons are registered into `PawnCombatComponent` via `RegisterSpawnedWeapon` — the tag passed becomes the lookup key.

## Key Module Dependencies

Declared in `PalaceManager.Build.cs`:
- `EnhancedInput` — input handling
- `GameplayTags`, `GameplayTasks` — GAS infrastructure (note: `GameplayAbilities` module is listed only in `.uproject`, not in Build.cs — add it if linking errors appear)
- `AIModule`, `NavigationSystem` — Behavior Tree and NavMesh
- `AnimGraphRuntime`, `MotionWarping` — animation systems


##  Git Commit Policy

- **Single Responsibility**: Keep commits small and focused (e.g., one commit for UI, one for Player state).
- **Prefixes**: Use `fix`, `feat`, `chore`, `refactor`, `docs`.
- **Messages**: Write concise commit messages in Korean.
- **No Signatures**: NEVER append "Co-authored-by" or Claude automatic signatures to git commits.

##  Git Branch & PR Policy (Critical)

- **Target Branch**: Always work on the `Refactor` branch.
- **No Direct Merges**: Never direct push or local merge to `main`.
- **Merge Process**: `Refactor -> main` integration MUST be done via GitHub Pull Request after review.
