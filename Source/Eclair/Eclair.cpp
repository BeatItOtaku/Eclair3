// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Eclair.h"
#include "Modules/ModuleManager.h"
#include "AssetTypeActions_EclairConv.h"

class FEclair : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	TSharedPtr<FAssetTypeActions_EclairConv> EclairConv_AssetTypeActions;
};

IMPLEMENT_PRIMARY_GAME_MODULE( FEclair, "Eclair", "Eclair" );

void FEclair::StartupModule()
{
	//EclairConversationのアセットメニュー登録
	EclairConv_AssetTypeActions = MakeShareable(new FAssetTypeActions_EclairConv);
	FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get().RegisterAssetTypeActions(EclairConv_AssetTypeActions.ToSharedRef());
}

void FEclair::ShutdownModule()
{
	//EclairConversationのアセットメニュー解除
	if (EclairConv_AssetTypeActions.IsValid())
	{
		if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
		{
			FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get()
				.UnregisterAssetTypeActions(EclairConv_AssetTypeActions.ToSharedRef());
		}
		EclairConv_AssetTypeActions.Reset();
	}
}
