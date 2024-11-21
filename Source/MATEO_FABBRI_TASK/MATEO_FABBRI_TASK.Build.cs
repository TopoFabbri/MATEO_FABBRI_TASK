// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MATEO_FABBRI_TASK : ModuleRules
{
	public MATEO_FABBRI_TASK(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
