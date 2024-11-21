using UnrealBuildTool;

public class GridModule: ModuleRules{
	public GridModule(ReadOnlyTargetRules Target) : base(Target)	{
		PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine", "GridMathModule", "GridTypesModule"});
	}
}