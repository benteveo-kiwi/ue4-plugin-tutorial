// Copyright Epic Games, Inc. All Rights Reserved.

#include "TutorialExample.h"
#include "TutorialExampleStyle.h"
#include "TutorialExampleCommands.h"
#include "ExampleMenuActions.h"

#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "ObjectTools.h"
#include "Misc/Paths.h"
#include "FileHelpers.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"



static const FName TutorialExampleTabName("TutorialExample");

#define LOCTEXT_NAMESPACE "FTutorialExampleModule"

void FTutorialExampleModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FTutorialExampleStyle::Initialize();
	FTutorialExampleStyle::ReloadTextures();

	FTutorialExampleCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FTutorialExampleCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FTutorialExampleModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FTutorialExampleModule::RegisterMenus));

	RegisterAssetTools();
}

void FTutorialExampleModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module. For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FTutorialExampleStyle::Shutdown();

	FTutorialExampleCommands::Unregister();

	UnregisterAssetTools();
}

void FTutorialExampleModule::PluginButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Entering world duplicate code."));

	UObject* worldTemplateObj = StaticLoadObject(UWorld::StaticClass(), NULL, TEXT("/TutorialExample/BaseLevel"));
	if (IsValid(worldTemplateObj)) 
	{
		ObjectTools::FPackageGroupName pgn;
		pgn.ObjectName = TEXT("BaseLevel");
		pgn.PackageName = TEXT("/Game/Maps/BaseLevel");
		TSet<UPackage*> objectsUserRefusedToFullyLoad;

		UObject* worldObj = ObjectTools::DuplicateSingleObject(worldTemplateObj, pgn, objectsUserRefusedToFullyLoad);
		if (IsValid(worldObj)) {
			UWorld* world = CastChecked<UWorld>(worldObj);
			FEditorFileUtils::SaveLevel(world->PersistentLevel);
			FEditorFileUtils::LoadMap(TEXT("/Game/Maps/BaseLevel"));
		} else {
			UE_LOG(LogTemp, Warning, TEXT("Could not duplicate object. Rejected by user?"));
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("World template invalid! Exiting."));
		return;
	}
}
	

void FTutorialExampleModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FTutorialExampleCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FTutorialExampleCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

void FTutorialExampleModule::RegisterAssetTools()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	auto Action = MakeShareable(new FExampleMenuActions());
	
	AssetTools.RegisterAssetTypeActions(Action);
	RegisteredAssetTypeActions.Add(Action);
}

void FTutorialExampleModule::UnregisterAssetTools()
{
	FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools");

	if (AssetToolsModule != nullptr)
	{
		IAssetTools& AssetTools = AssetToolsModule->Get();

		for (auto Action : RegisteredAssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(Action);
		}
	}
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTutorialExampleModule, TutorialExample)