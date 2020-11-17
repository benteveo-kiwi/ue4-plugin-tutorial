#include "ExampleMenuActions.h"

#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Styling/SlateStyle.h"

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

	auto TextAssets = GetTypedWeakObjectPtrs<UObject>(InObjects);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("TextAsset_ReverseText", "Reverse Text"),
		LOCTEXT("TextAsset_ReverseTextToolTip", "Reverse the text stored in the selected text asset(s)."),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateLambda([=] {
				UE_LOG(LogTemp, Warning, TEXT("Right click OK."));
				/*for (auto& TextAsset : TextAssets)
				{
					if (TextAsset.IsValid() && !TextAsset->Text.IsEmpty())
					{
						TextAsset->Text = FText::FromString(TextAsset->Text.ToString().Reverse());
						TextAsset->PostEditChange();
						TextAsset->MarkPackageDirty();
					}
				}*/
				}),
			FCanExecuteAction::CreateLambda([=] {
					UE_LOG(LogTemp, Warning, TEXT("Checking if should show in menu."));
					return true;
					/*for (auto& TextAsset : TextAssets)
					{
						if (TextAsset.IsValid() && !TextAsset->Text.IsEmpty())
						{
							return true;
						}
					}
					return false;*/
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
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_TextAsset", "Text Asset");
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
	
	/*EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid()
		? EToolkitMode::WorldCentric
		: EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		auto TextAsset = Cast<UTextAsset>(*ObjIt);

		if (TextAsset != nullptr)
		{
			TSharedRef<FTextAssetEditorToolkit> EditorToolkit = MakeShareable(new FTextAssetEditorToolkit(Style));
			EditorToolkit->Initialize(TextAsset, Mode, EditWithinLevelEditor);
		}
	}*/
}


#undef LOCTEXT_NAMESPACE