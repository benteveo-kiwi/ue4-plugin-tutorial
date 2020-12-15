// Copyright Epic Games, Inc. All Rights Reserved.

#include "TutorialExample.h"
#include "TutorialExampleStyle.h"
#include "TutorialExampleCommands.h"
#include "ExampleMenuActions.h"
#include "TutorialExampleSettings.h"

#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "ObjectTools.h"
#include "Misc/Paths.h"
#include "FileHelpers.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "GameMapsSettings.h"
#include "GameFramework/InputSettings.h"

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
	UE_LOG(LogTemp, Log, TEXT("Entering World duplicate code."));

	FText DialogText = FText::FromString("This action will replace your default levels and create new keybindings. Continue?");
	EAppReturnType::Type ReturnValue = FMessageDialog::Open(EAppMsgType::YesNo, DialogText);

	if (ReturnValue == EAppReturnType::No)
	{
		return;
	}

	UObject* worldTemplateObj = StaticLoadObject(UWorld::StaticClass(), NULL, *TutorialExampleSettings::BaseLevelTemplate);
	if (IsValid(worldTemplateObj)) 
	{
		ObjectTools::FPackageGroupName pgn;
		pgn.ObjectName = TEXT("BaseLevel");
		pgn.PackageName = TutorialExampleSettings::BaseLevel;
		TSet<UPackage*> objectsUserRefusedToFullyLoad;

		UObject* worldObj = ObjectTools::DuplicateSingleObject(worldTemplateObj, pgn, objectsUserRefusedToFullyLoad);
		if (IsValid(worldObj)) {
			UWorld* World = CastChecked<UWorld>(worldObj);
			FEditorFileUtils::SaveLevel(World->PersistentLevel);
			FEditorFileUtils::LoadMap(TutorialExampleSettings::BaseLevel);
			ChangeDefaultLevels(World);
			CreateNewKeybindings();
		} else {
			UE_LOG(LogTemp, Warning, TEXT("Could not duplicate object. Rejected by user?"));
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("World template invalid! Exiting."));
		return;
	}
}

void FTutorialExampleModule::ChangeDefaultLevels(UWorld * BaseLevel) 
{
	UE_LOG(LogTemp, Log, TEXT("Changing default levels."));

	UGameMapsSettings::SetGameDefaultMap(BaseLevel->GetPathName());

	UGameMapsSettings* Settings = UGameMapsSettings::GetGameMapsSettings();
	Settings->EditorStartupMap = BaseLevel;
}

void FTutorialExampleModule::CreateNewKeybindings() 
{
	UE_LOG(LogTemp, Log, TEXT("Changing default levels."));
	
	CreateNewKeybinding(FName(TEXT("OurPlugin_ChangeLocation")), FKey(TEXT("Q")));
	CreateNewKeybinding(FName(TEXT("OurPlugin_ChangeLevel")), FKey(TEXT("W")));
}

void FTutorialExampleModule::CreateNewKeybinding(FName Name, FKey Key) 
{
	UInputSettings* Settings = UInputSettings::GetInputSettings();

	Settings->RemoveActionMapping(FInputActionKeyMapping(Name, Key));
	Settings->AddActionMapping(FInputActionKeyMapping(Name, Key));
	Settings->SaveKeyMappings();
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