// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FTutorialExampleModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();

	void ChangeDefaultLevels(UWorld* BaseLevel);

	void CreateNewKeybindings();

	void CreateNewKeybinding(FName Name, FKey Key);
	
private:

	void RegisterMenus();
	void RegisterAssetTools();
	void UnregisterAssetTools();


private:
	TSharedPtr<class FUICommandList> PluginCommands;
	TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;
};
