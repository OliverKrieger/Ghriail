using UnrealBuildTool;

public class NPCModule: ModuleRules{
	public NPCModule(ReadOnlyTargetRules Target) : base(Target)	{
		PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine"});
	}
}