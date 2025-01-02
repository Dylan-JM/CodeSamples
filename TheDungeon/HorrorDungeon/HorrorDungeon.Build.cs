// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HorrorDungeon : ModuleRules
{
	public HorrorDungeon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "GameplayCameras", "GameplayAbilities", "FieldNotification", "AIModule", "OnlineSubsystem", "OnlineSubsystemSteam" });
		
		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayTags", "GameplayTasks", "NavigationSystem", "Niagara", "AnimGraphRuntime" });
		
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
	}
}
