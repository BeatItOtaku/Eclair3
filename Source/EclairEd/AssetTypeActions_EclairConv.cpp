#include "AssetTypeActions_EclairConv.h"
#include "EclairEd.h"
#include "EditorStyleSet.h"
#include "Slate/SlateBrushAsset.h"
#include "Styling/SlateColor.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Images/SImage.h"
#include "EclairConversation.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

TSharedPtr<SWidget> FAssetTypeActions_EclairConv::GetThumbnailOverlay(const FAssetData & AssetData) const
{
	TSharedPtr<SBox> Box;
	SAssignNew(Box, SBox)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Top)
		.Padding(FMargin(2));
	TSharedPtr<SImage> Image;
	SAssignNew(Image, SImage).Image_Lambda([this, AssetData]() -> const FSlateBrush* {return &(LoadObject<USlateBrushAsset>(NULL, TEXT("/Game/Base/EclairConversation/Conversation_Brush.Conversation_Brush"), NULL, LOAD_None, NULL)->Brush); });
	Box->SetContent(Image.ToSharedRef());
	Box->SetVisibility(EVisibility::Visible);
	return Box;
}

UClass* FAssetTypeActions_EclairConv::GetSupportedClass() const
{
	return UEclairConversation::StaticClass();
}

void FAssetTypeActions_EclairConv::GetResolvedSourceFilePaths(const TArray<UObject*>& TypeAssets, TArray<FString>& OutSourceFilePaths) const
{
    for (auto& Asset : TypeAssets)
    {
        UEclairConversation* Conversation = CastChecked<UEclairConversation>(Asset);
        Conversation->AssetImportData->ExtractFilenames(OutSourceFilePaths);
    }
}

#undef LOCTEXT_NAMESPACE
