// Copyright Epic Games, Inc. All Rights Reserved.

#include "TutorialExampleCommands.h"

#define LOCTEXT_NAMESPACE "FTutorialExampleModule"

void FTutorialExampleCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "TutorialExample", "Execute TutorialExample action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
