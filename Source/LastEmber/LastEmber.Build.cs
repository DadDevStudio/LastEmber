// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LastEmber : ModuleRules
{
	public LastEmber(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"LastEmber",
			"LastEmber/Variant_Platforming",
			"LastEmber/Variant_Platforming/Animation",
			"LastEmber/Variant_Combat",
			"LastEmber/Variant_Combat/AI",
			"LastEmber/Variant_Combat/Animation",
			"LastEmber/Variant_Combat/Gameplay",
			"LastEmber/Variant_Combat/Interfaces",
			"LastEmber/Variant_Combat/UI",
			"LastEmber/Variant_SideScrolling",
			"LastEmber/Variant_SideScrolling/AI",
			"LastEmber/Variant_SideScrolling/Gameplay",
			"LastEmber/Variant_SideScrolling/Interfaces",
			"LastEmber/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
