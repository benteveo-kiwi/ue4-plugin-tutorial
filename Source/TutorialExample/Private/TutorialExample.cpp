// Copyright Epic Games, Inc. All Rights Reserved.

#include "TutorialExample.h"
#include "TutorialExampleStyle.h"
#include "TutorialExampleCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "ObjectTools.h"

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
}

void FTutorialExampleModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FTutorialExampleStyle::Shutdown();

	FTutorialExampleCommands::Unregister();
}

void FTutorialExampleModule::PluginButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Entering world duplicate code."));
	UObject* worldTemplateObj = StaticLoadObject(UWorld::StaticClass(), NULL, TEXT("/TutorialExample/BaseLevel"));
	if (IsValid(worldTemplateObj)) 
	{
		ObjectTools::FPackageGroupName pgn;
		pgn.ObjectName = TEXT("BaseWorld");
		pgn.PackageName = TEXT("/Game/Maps/BaseLevel-COPY");
		TSet<UPackage*> objectsUserRefusedToFullyLoad;

		UObject* worldObj = ObjectTools::DuplicateSingleObject(worldTemplateObj, pgn, objectsUserRefusedToFullyLoad);
		if (IsValid(worldObj)) {
			UWorld* world = CastChecked<UWorld>(worldObj);
		} else {
			UE_LOG(LogTemp, Warning, TEXT("Could not duplicate object."));
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("World template invalid! Exiting."));
		return;
	}
}
	/*FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FTutorialExampleModule::PluginButtonClicked()")),
							FText::FromString(TEXT("TutorialExample.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);*/

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

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTutorialExampleModule, TutorialExample)