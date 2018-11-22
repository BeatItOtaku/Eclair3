// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EclairEd.h"
#include "Modules/ModuleManager.h"
#include "AssetTypeActions_EclairConv.h"

class FEclairEd : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	TSharedPtr<FAssetTypeActions_EclairConv> EclairConv_AssetTypeActions;
};

IMPLEMENT_GAME_MODULE( FEclairEd, "EclairEd");

void FEclairEd::StartupModule()
{
	//EclairConversation�̃A�Z�b�g���j���[�o�^
	EclairConv_AssetTypeActions = MakeShareable(new FAssetTypeActions_EclairConv);
	FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get().RegisterAssetTypeActions(EclairConv_AssetTypeActions.ToSharedRef());
}

void FEclairEd::ShutdownModule()
{
	//EclairConversation�̃A�Z�b�g���j���[����
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