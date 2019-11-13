using UnrealBuildTool;
using System.IO;

public class UniversalLogging : ModuleRules
{
    public UniversalLogging(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateIncludePaths.AddRange(new string[] { "UniversalLogging/Private" });
        PublicIncludePaths.AddRange(new string[] { "UniversalLogging/Public" });

        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Core" });
    }
}