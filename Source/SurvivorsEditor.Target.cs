// Optics Team

using UnrealBuildTool;
using System.Collections.Generic;

public class SurvivorsEditorTarget : TargetRules
{
	public SurvivorsEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Survivors" } );
	}
}
