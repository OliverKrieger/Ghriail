using UnrealBuildTool;

public class WFCModule: ModuleRules{
	public WFCModule(ReadOnlyTargetRules Target) : base(Target)	{
		PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine"});
	}
}