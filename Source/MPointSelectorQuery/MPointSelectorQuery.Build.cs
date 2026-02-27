// Copyright (c) Miknios. All rights reserved.

using UnrealBuildTool;

public class MPointSelectorQuery : ModuleRules
{
	public MPointSelectorQuery(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"MUtility",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"PhysicsCore",
				"AIModule",
			}
		);
	}
}

