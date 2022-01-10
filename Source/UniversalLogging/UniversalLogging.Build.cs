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

        PublicDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine", "Core" });
        
        if (Target.Platform == UnrealTargetPlatform.Linux || Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicDependencyModuleNames.AddRange(new string[] { "DisplayCluster" });
        }

        //this is needed to register on Editor delegates, i.e., BeginPIE and EndPIE, but only in Editor builds
        if (Target.Type == TargetRules.TargetType.Editor)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" });
        }
    }
}