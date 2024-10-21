using UnrealBuildTool;

public class PathfindingModule: ModuleRules{
	public PathfindingModule(ReadOnlyTargetRules Target) : base(Target)	{
		PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine"});
	}
}