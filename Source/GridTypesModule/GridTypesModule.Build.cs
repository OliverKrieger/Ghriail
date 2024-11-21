using UnrealBuildTool;

public class GridTypesModule: ModuleRules{
	public GridTypesModule(ReadOnlyTargetRules Target) : base(Target)	{
		PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine"});
	}
}