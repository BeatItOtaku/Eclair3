// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealEd.h"
#include "AssetTypeActions_Base.h"

/**
 * 
 */
class ECLAIRED_API FAssetTypeActions_EclairConv : public FAssetTypeActions_Base
{
public:
	virtual FText GetName() const override
	{
		//return FText::FromString("");
		return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_EclairConv", "Eclair\nConversation");
	}
	virtual TSharedPtr<SWidget> GetThumbnailOverlay(const FAssetData& AssetData) const override;
	virtual FColor GetTypeColor() const override { return FColor::Orange; }
	virtual uint32 GetCategories() override { return EAssetTypeCategories::Misc; }
	virtual UClass* GetSupportedClass() const override;
	virtual bool IsImportedAsset() const override { return true; }
    virtual void GetResolvedSourceFilePaths(const TArray<UObject*>& TypeAssets, TArray<FString>& OutSourceFilePaths) const override;
};
