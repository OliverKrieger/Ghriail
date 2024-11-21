using UnrealBuildTool;

public class GameplayModule: ModuleRules{
	public GameplayModule(ReadOnlyTargetRules Target) : base(Target)	{
		PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine"});
	}
}