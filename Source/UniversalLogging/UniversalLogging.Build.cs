using UnrealBuildTool;
using System.IO;

public class UniversalLogging : ModuleRules
{
    public UniversalLogging(ReadOnlyTargetRules Target) : base(Target)
    {
        bLegacyPublicIncludePaths = false;

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.AddRange(new string[] {  });
        PublicIncludePaths.AddRange(new string[] {  });

        PublicDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine", "Core", "DisplayCluster" });
    }
}