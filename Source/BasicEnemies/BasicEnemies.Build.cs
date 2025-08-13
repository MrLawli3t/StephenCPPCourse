using UnrealBuildTool;

public class BasicEnemies : ModuleRules
{
    public BasicEnemies(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
                "BasicCharacters"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine", 
                "AIModule",
                "ItemsAndWeapons"
            }
        );
    }
}