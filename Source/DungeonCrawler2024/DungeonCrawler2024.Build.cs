// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DungeonCrawler2024 : ModuleRules
{
	public DungeonCrawler2024(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
