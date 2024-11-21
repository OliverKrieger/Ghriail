using UnrealBuildTool;

public class GridMathModule: ModuleRules{
	public GridMathModule(ReadOnlyTargetRules Target) : base(Target)	{
		PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine"});
	}
}