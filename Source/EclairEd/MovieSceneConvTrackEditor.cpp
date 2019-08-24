// Fill out your copyright notice in the Description page of Project Settings.

#include "MovieSceneConvTrackEditor.h"

#include "Misc/Guid.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "EditorStyleSet.h"
#include "SequencerUtilities.h"
#include "SequencerSectionPainter.h"

#include "MovieSceneConversationSection.h"

#include "RenderUtils.h"
#include "Textures/SlateTextureData.h"
#include "Slate/SlateTextures.h"
#include "Rendering/DrawElements.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/Application/SlateApplication.h"

#include "MovieScene.h"

#define LOCTEXT_NAMESPACE "MovieSceneConvTrackEditor"

class FMovieSceneConversationSection
	: public ISequencerSection
{
public:

	FMovieSceneConversationSection(UMovieSceneSection& InSection, TWeakPtr<ISequencer> InSequencer)
		: Section(Cast<UMovieSceneConversationSection>(&InSection))
		, Sequencer(InSequencer)
	{
	}

	~FMovieSceneConversationSection()
	{
	}
public:

	// ISequencerSection interface

	virtual UMovieSceneSection* GetSectionObject() override { return Section; }

#if !UE_4_17_OR_LATER
	virtual FText GetDisplayName() const override
	{
		return LOCTEXT("DisplayName", "AkAudioEvent");
	}
#endif // !UE_4_17_OR_LATER

	virtual FText GetSectionTitle() const override
	{
		/*if (Section != nullptr)
		{
			FString name = Section->GetEventName();
			if (Section->EventTracker->IsDirty)
				name += "*";
			return FText::FromString(name);
		}*/
		return FText::GetEmpty();
	}

	virtual int32 OnPaintSection(FSequencerSectionPainter& InPainter) const override
	{
		return InPainter.PaintSectionBackground();
	}

#if !UE_4_20_OR_LATER
	virtual void GenerateSectionLayout(class ISectionLayoutBuilder& LayoutBuilder) const override {}
#endif

#if !UE_4_20_OR_LATER
	virtual void ResizeSection(ESequencerSectionResizeMode ResizeMode, float ResizeTime) override
	{
		if (ResizeMode == ESequencerSectionResizeMode::SSRM_TrailingEdge)
		{
			//Section->SetEndTime(ResizeTime);
		}
	}
#endif

	/**
	* Builds up the section context menu for the outliner
	*
	* @param MenuBuilder	The menu builder to change
	* @param ObjectBinding The object guid bound to this section
	*/
	virtual void BuildSectionContextMenu(FMenuBuilder& MenuBuilder, const FGuid& ObjectBinding) override
	{

	}

private:
	/* Callback handles. */
	FDelegateHandle ProjectLoadedHandle;
	FDelegateHandle ConnectionLostHandle;
	FDelegateHandle SoundbanksLoadedHandle;

	/** The section we are visualizing */
	UMovieSceneConversationSection* Section;

	/** The amount of pixels to offset the waveform texture by (if any part of it is clipped beyond the left of the editor view). */
	float PixelOffsetLeft = 0.0f;
	/** The number of pixels "left over" at the end of the smoothed waveform. This will be between 0 and AkAudioWaveformViewport::SmoothingAmount. */
	int LeftOverPixels = 0;
	/** This depends on the zoom level, as well as the position and length of the section*/
	int NumPeaksRequired = 0;

	TWeakPtr<ISequencer> Sequencer;
};

FMovieSceneConvTrackEditor::FMovieSceneConvTrackEditor(TSharedRef<ISequencer> InSequencer)
	: FMovieSceneTrackEditor(InSequencer)
{ }


TSharedRef<ISequencerTrackEditor> FMovieSceneConvTrackEditor::CreateTrackEditor(TSharedRef<ISequencer> InSequencer)
{
	return MakeShareable(new FMovieSceneConvTrackEditor(InSequencer));
}

bool FMovieSceneConvTrackEditor::SupportsSequence(UMovieSceneSequence* InSequence) const
{
	// Level Sequence のみ許可
	return (InSequence != nullptr) && (InSequence->GetClass()->GetName() == TEXT("LevelSequence"));
}

bool FMovieSceneConvTrackEditor::SupportsType(TSubclassOf<UMovieSceneTrack> Type) const
{
	return Type == UMovieSceneConversationTrack::StaticClass();
}

TSharedRef<ISequencerSection> FMovieSceneConvTrackEditor::MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding)
{
	return MakeShareable(new FMovieSceneConversationSection(SectionObject, GetSequencer()));
}

const FSlateBrush* FMovieSceneConvTrackEditor::GetIconBrush() const
{
	// イベントトラックと同じアイコンにしておく
	return FEditorStyle::GetBrush("Sequencer.Tracks.Event");
}

void FMovieSceneConvTrackEditor::BuildObjectBindingTrackMenu(FMenuBuilder& MenuBuilder, const FGuid& ObjectBinding, const UClass* ObjectClass)
{
	// 念のためアクター限定としておく
	if (true)//ObjectClass->IsChildOf(AActor::StaticClass()))
	{
		MenuBuilder.AddMenuEntry(
			LOCTEXT("AddConversationTrack", "ConversationTrack"),
			LOCTEXT("AddConversationTrackTooltip", "Add an Conversation Track."),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateSP(this, &FMovieSceneConvTrackEditor::FindOrCreateTrack, ObjectBinding)
			)
		);
	}
}

TSharedPtr<SWidget> FMovieSceneConvTrackEditor::BuildOutlinerEditWidget(const FGuid& ObjectBinding, UMovieSceneTrack* Track, const FBuildEditWidgetParams& Params)
{
	// Create a container edit box
	return SNew(SHorizontalBox)

		// Add the audio combo box
		+ SHorizontalBox::Slot()
		.AutoWidth();
		/*.VAlign(VAlign_Center)
		[
			FSequencerUtilities::MakeAddButton(LOCTEXT("AudioText", "AkAudioEvent"), FOnGetContent::CreateSP(this, &FMovieSceneConvTrackEditor::BuildAudioSubMenu, Track), Params.NodeIsHovered)
		];*/
}

void FMovieSceneConvTrackEditor::FindOrCreateTrack(FGuid ObjectHandle)
{
	// トラックがなかったら作成する
	FFindOrCreateTrackResult TrackResult = FindOrCreateTrackForObject(ObjectHandle, UMovieSceneConversationTrack::StaticClass(), TEXT("Actor Event"));
	auto* NewTrack = CastChecked<UMovieSceneConversationTrack>(TrackResult.Track, ECastCheckedType::NullAllowed);

	if (TrackResult.bWasCreated)
	{
		// トラック作成成功

		// セクション作成
		UMovieSceneSection* NewSection = NewTrack->CreateNewSection();
		check(NewSection);

		// セクション追加
		NewTrack->AddSection(*NewSection);
		NewTrack->SetDisplayName(LOCTEXT("TrackName", "Conversation"));

		//テスト
		NewTrack->AddNewEvent(0, nullptr);
	}

	// 構成に変更が起きたことを Sequencer に伝える(トラックが再描画される）
	GetSequencer()->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
}

#undef LOCTEXT_NAMESPACE