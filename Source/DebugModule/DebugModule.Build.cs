using UnrealBuildTool;

public class DebugModule: ModuleRules{
	public DebugModule(ReadOnlyTargetRules Target) : base(Target)	{
		PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine", "GridModule"});
	}
}