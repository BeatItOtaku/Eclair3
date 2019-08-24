// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EclairEd.h"
#include "Modules/ModuleManager.h"
#include "AssetTypeActions_EclairConv.h"
#include "ISettingsModule.h"
#include "EclairConversationSetting.h"

#include "MovieSceneConvTrackEditor.h"
#include "ISequencerModule.h"

#define LOCTEXT_NAMESPACE ""

class FEclairEd : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	TSharedPtr<FAssetTypeActions_EclairConv> EclairConv_AssetTypeActions;
	FDelegateHandle ConversationTrackEditorHandle;
};

IMPLEMENT_GAME_MODULE( FEclairEd, EclairEd);

void FEclairEd::StartupModule()
{
	//EclairConversationのアセットメニュー登録
	EclairConv_AssetTypeActions = MakeShareable(new FAssetTypeActions_EclairConv);
	FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get().RegisterAssetTypeActions(EclairConv_AssetTypeActions.ToSharedRef());

	//EclairConversationの設定項目
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule -> RegisterSettings(
			"Project", // ※２
			"EclairCategory",
			"Eclair",
			LOCTEXT("EclairConversationSettingName", "EclairConversationSetting"),
			LOCTEXT("EclairConversationSettingDescription", "EclairConversation discription"),
			GetMutableDefault<UEclairConversationSetting>()
		);
	}

	//ConversationTrack
	ISequencerModule& SequencerModule = FModuleManager::Get().LoadModuleChecked<ISequencerModule>("Sequencer");
	ConversationTrackEditorHandle = SequencerModule.RegisterTrackEditor(
		FOnCreateTrackEditor::CreateStatic(&FMovieSceneConvTrackEditor::CreateTrackEditor));
}

void FEclairEd::ShutdownModule()
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

	//EclairConversationの設定項目解除
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule -> UnregisterSettings(
			"Project", // ※３
			"EclairCategory",
			"Eclair"
		);
	}

	//ConversationTrack
	if (!FModuleManager::Get().IsModuleLoaded("Sequencer"))
	{
		return;
	}

	ISequencerModule& SequencerModule = FModuleManager::Get().GetModuleChecked<ISequencerModule>("Sequencer");
	SequencerModule.UnRegisterTrackEditor(ConversationTrackEditorHandle);
}

#undef LOCTEXT_NAMESPACE