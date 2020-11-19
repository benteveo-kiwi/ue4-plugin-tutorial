#include "ExampleMenuActions.h"
#include "TutorialExampleSettings.h"

#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Styling/SlateStyle.h"
#include "EditorLevelUtils.h"
#include "Engine/LevelStreamingDynamic.h"
#include "FileHelpers.h"
#include "EditorModeManager.h"


#define LOCTEXT_NAMESPACE "AssetTypeActions"


/* FExampleMenuActions constructors
 *****************************************************************************/

FExampleMenuActions::FExampleMenuActions()
{ }


/* FAssetTypeActions_Base overrides
 *****************************************************************************/

bool FExampleMenuActions::CanFilter()
{
	return true;
}

void FExampleMenuActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
	FAssetTypeActions_Base::GetActions(InObjects, MenuBuilder);

	auto WorldAssets = GetTypedWeakObjectPtrs<UWorld>(InObjects);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("TextAsset_ReverseText", "Add Sublevel"),
		LOCTEXT("TextAsset_ReverseTextToolTip", "Adds a sublevel to the base level."),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateLambda([=] {
				UE_LOG(LogTemp, Warning, TEXT("Add sublevel clicked."));

				// Check that the "BaseLevel" has been creaeted at some point.
				UWorld* BaseLevel = CastChecked<UWorld>(StaticLoadObject(UWorld::StaticClass(), NULL, *TutorialExampleSettings::BaseLevel));
				if (!IsValid(BaseLevel)) {
					FText DialogText = FText::FromString("Base level doesn't exist. Please create using 'Create Base Level' button.");
					FMessageDialog::Open(EAppMsgType::Ok, DialogText);
					return;
				}

				// Check that the currently open level is "BaseLevel".
				FEditorModeTools& EditorTools = GLevelEditorModeTools();
				UWorld* CurrentlyOpenWorld = EditorTools.GetWorld();
				if (CurrentlyOpenWorld->GetMapName() != FString(TEXT("BaseLevel")))
				{
					FText DialogText = FText::FromString("This functionality can only be invoked if BaseLevel is currently open. Open the 'BaseLevel' level and try again.");
					FMessageDialog::Open(EAppMsgType::Ok, DialogText);
					return;
				}

				// Add all levels.
				for (auto& World : WorldAssets) {
					if (World.IsValid())
					{
						FString PathName = World->GetPathName();
						UE_LOG(LogTemp, Warning, TEXT("Processing '%s'."), *PathName);

						ULevelStreaming* AddedLevel = UEditorLevelUtils::AddLevelToWorld(BaseLevel, *PathName, ULevelStreamingDynamic::StaticClass());
						if (!IsValid(AddedLevel))
						{
							UE_LOG(LogTemp, Warning, TEXT("Could not add level!"));
						}
						else 
						{
							UE_LOG(LogTemp, Warning, TEXT("Level added successfully."));
						}
					}
				}
			}),
			FCanExecuteAction::CreateLambda([=] {
					for (auto& World : WorldAssets)
					{
						if (World.IsValid()) {
							// Cannot add BaseLevel to itself.
							if (World->GetMapName() == FString(TEXT("BaseLevel")))
							{
								return false;
							}
						}
					}
					return true;
				})
			)
	);
}
uint32 FExampleMenuActions::GetCategories()
{
	return EAssetTypeCategories::Misc;
}


FText FExampleMenuActions::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_Map", "Map");
}


UClass* FExampleMenuActions::GetSupportedClass() const
{
	return UWorld::StaticClass();
}


FColor FExampleMenuActions::GetTypeColor() const
{
	return FColor::White;
}


bool FExampleMenuActions::HasActions(const TArray<UObject*>& InObjects) const
{
	return true;
}


void FExampleMenuActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	/// <summary>
	/// The purpose of this function is normally to create your own editor for a new asset type. Because an asset editor already exists for levels, we simply call that instead.
	/// 
	/// If multiple objects are passed to this function, only the first map is opened.
	/// </summary>
	/// <param name="InObjects"></param>
	/// <param name="EditWithinLevelEditor"></param>
	
	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		UWorld * World = Cast<UWorld>(*ObjIt);

		if (IsValid(World))
		{
			FString PathnameWithType = World->GetFullName();
			FString Left;
			FString Right;

			bool Success = PathnameWithType.Split(TEXT(" "), &Left, &Right);
			UE_LOG(LogTemp, Warning, TEXT("Processing '%s'."), *Right);
			FEditorFileUtils::LoadMap(Right);
			return;
		}
	}
}




#undef LOCTEXT_NAMESPACE