using UnrealBuildTool;

public class Pathfinding: ModuleRules{
	public Pathfinding(ReadOnlyTargetRules Target) : base(Target)	{
		PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine"});
	}
}