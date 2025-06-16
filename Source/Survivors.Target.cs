// Optics Team

using UnrealBuildTool;
using System.Collections.Generic;

public class SurvivorsTarget : TargetRules
{
	public SurvivorsTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Survivors" } );
	}
}
