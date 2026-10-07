// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AuthClientServerMod : ModuleRules
{
	public AuthClientServerMod(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate",
			"NetCore" // Requerido para macros avanzadas de red
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"AuthClientServerMod",
			"AuthClientServerMod/Variant_Platforming",
			"AuthClientServerMod/Variant_Platforming/Animation",
			"AuthClientServerMod/Variant_Combat",
			"AuthClientServerMod/Variant_Combat/AI",
			"AuthClientServerMod/Variant_Combat/Animation",
			"AuthClientServerMod/Variant_Combat/Gameplay",
			"AuthClientServerMod/Variant_Combat/Interfaces",
			"AuthClientServerMod/Variant_Combat/UI",
			"AuthClientServerMod/Variant_SideScrolling",
			"AuthClientServerMod/Variant_SideScrolling/AI",
			"AuthClientServerMod/Variant_SideScrolling/Gameplay",
			"AuthClientServerMod/Variant_SideScrolling/Interfaces",
			"AuthClientServerMod/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
