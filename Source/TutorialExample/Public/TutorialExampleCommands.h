// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "TutorialExampleStyle.h"

class FTutorialExampleCommands : public TCommands<FTutorialExampleCommands>
{
public:

	FTutorialExampleCommands()
		: TCommands<FTutorialExampleCommands>(TEXT("TutorialExample"), NSLOCTEXT("Contexts", "TutorialExample", "TutorialExample Plugin"), NAME_None, FTutorialExampleStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
