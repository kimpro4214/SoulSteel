// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PalaceManager : ModuleRules
{
	public PalaceManager(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayTags", "GameplayTasks", "AIModule", "AnimGraphRuntime", "MotionWarping", "NavigationSystem" });
	}
}
