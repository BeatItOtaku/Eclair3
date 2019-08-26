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
#include "Modules/ModuleManager.h"
#include "IDetailCustomization.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Images/SImage.h"
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
		if (Section != nullptr)
		{
			FString name = Section->Item.Content;
			return FText::FromString(name);
		}
		return FText::GetEmpty();
	}

	virtual float GetSectionHeight() const override
	{
		return 31;
	}

	virtual int32 OnPaintSection(FSequencerSectionPainter& InPainter) const override
	{
		int32 LayerId = InPainter.PaintSectionBackground();
		if (Section->isSectionHead) {
			LayerId++;
			FPaintGeometry PaintGeometry = InPainter.SectionGeometry.ToPaintGeometry(FVector2D(1.f, 3.f), FVector2D(16.0f,30.0f));
			const FSlateBrush* brush = FEditorStyle::GetBrush("Sequencer.Tracks.Event");
			FSlateDrawElement::MakeBox(InPainter.DrawElements, LayerId, PaintGeometry, brush);
		}
		return LayerId;
	}

#if !UE_4_20_OR_LATER
	virtual void GenerateSectionLayout(class ISectionLayoutBuilder& LayoutBuilder) const override {}
#endif

#if !UE_4_20_OR_LATER
	virtual void ResizeSection(ESequencerSectionResizeMode ResizeMode, float ResizeTime) override
	{
		TArray<UMovieSceneSection *> sections = GetSectionObject()->GetTypedOuter<UMovieSceneTrack>()->GetAllSections();
		int32 index = sections.Find(Section);
		if (ResizeMode == ESequencerSectionResizeMode::SSRM_LeadingEdge)
		{
			//空白をなくす
			if (index > 0) {
				ResizeTime = FMath::Max(ResizeTime, sections[index - 1]->GetStartTime());
				sections[index - 1]->SetEndTime(ResizeTime);
			}
			Section->SetStartTime(ResizeTime);
		}
		if (ResizeMode == ESequencerSectionResizeMode::SSRM_TrailingEdge)
		{
			//空白をなくす
			if (index + 1 < sections.Num()) {
				ResizeTime = FMath::Min(ResizeTime, sections[index + 1]->GetEndTime());
				sections[index + 1]->SetStartTime(ResizeTime);
			}
			Section->SetEndTime(ResizeTime);
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

//自動でConversationを割り当てるようになったらこのコードはいらないはずである
void FMovieSceneConvTrackEditor::BuildTrackContextMenu(FMenuBuilder & MenuBuilder, UMovieSceneTrack * Track)
{
	UMovieSceneConversationTrack* ConvTrack = CastChecked<UMovieSceneConversationTrack>(Track);

	/** Specific details customization for the event track */
	class FConversationTrackCustomization : public IDetailCustomization
	{
	public:
		UMovieSceneConversationTrack *track;

		FConversationTrackCustomization() {}
		FConversationTrackCustomization(UMovieSceneConversationTrack *Track) : track(Track) {}

		void OnPropertyValueChanged() {
			if(track != nullptr) track->RemapSectionsItem();
		}

		virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
		{
			DetailBuilder.HideCategory("Track");
			DetailBuilder.HideCategory("General");

			//track = DetailBuilder.GetBaseClass()->GetTypedOuter<UMovieSceneConversationTrack>();
			//volatile auto hoge = DetailBuilder.GetBaseClass();

			IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("ConversationTrack");
			TSharedPtr<IPropertyHandle> handle = Category.AddProperty("Conversation").ShouldAutoExpand(true).GetPropertyHandle();
			handle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this,&FConversationTrackCustomization::OnPropertyValueChanged));
		}
	};

	auto PopulateSubMenu = [this, ConvTrack](FMenuBuilder& SubMenuBuilder)
	{
		FPropertyEditorModule& PropertyEditor = FModuleManager::Get().LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

		// Create a details view for the track
		FDetailsViewArgs DetailsViewArgs(false, false, false, FDetailsViewArgs::HideNameArea, true);
		DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Automatic;
		DetailsViewArgs.bShowOptions = false;
		//DetailsViewArgs.ColumnWidth = 0.55f;

		TSharedRef<IDetailsView> DetailsView = PropertyEditor.CreateDetailView(DetailsViewArgs);

		// Register the custom type layout for the class
		FOnGetDetailCustomizationInstance CreateInstance = FOnGetDetailCustomizationInstance::CreateLambda([ConvTrack]() {return MakeShared<FConversationTrackCustomization>(ConvTrack); });
		DetailsView->RegisterInstancedCustomPropertyLayout(UMovieSceneConversationTrack::StaticClass(), CreateInstance);

		GetSequencer()->OnInitializeDetailsPanel().Broadcast(DetailsView, GetSequencer().ToSharedRef());

		// Assign the object
		DetailsView->SetObject(ConvTrack, true);

		// Add it to the menu
		TSharedRef< SWidget > DetailsViewWidget =
			SNew(SBox)
			.MaxDesiredHeight(400.0f)
			.WidthOverride(450.0f)
			[
				DetailsView
			];

		SubMenuBuilder.AddWidget(DetailsViewWidget, FText(), true, false);
	};

	MenuBuilder.AddSubMenu(LOCTEXT("Properties_MenuText", "Properties"), FText(), FNewMenuDelegate::CreateLambda(PopulateSubMenu));
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
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			//FSequencerUtilities::MakeAddButton(LOCTEXT("ConversationItem", "Item"), FOnGetContent::CreateSP(this, &FMovieSceneConvTrackEditor::OnCreateButtonClicked, Track), Params.NodeIsHovered)
			//MakeAddButton(LOCTEXT("ConversationItem", "Item"), FOnClicked::CreateRaw(this, &FMovieSceneConvTrackEditor::OnCreateButtonClicked, Track), Params.NodeIsHovered)
			//TODO: Conversationの展開ボタンにして横並び
			MakeAddButton(LOCTEXT("ConversationInflate", "Inflate"), FOnClicked::CreateRaw(this, &FMovieSceneConvTrackEditor::OnCreateButtonClicked, Track), Params.NodeIsHovered)
		];
}

void FMovieSceneConvTrackEditor::FindOrCreateTrack(FGuid ObjectHandle)
{
	// トラックがなかったら作成する
	FFindOrCreateTrackResult TrackResult = FindOrCreateTrackForObject(ObjectHandle, UMovieSceneConversationTrack::StaticClass(), TEXT("Conversation"));
	auto* NewTrack = CastChecked<UMovieSceneConversationTrack>(TrackResult.Track, ECastCheckedType::NullAllowed);

	if (TrackResult.bWasCreated)
	{
		NewTrack->SetDisplayName(LOCTEXT("TrackName", "Conversation"));
	}

	// 構成に変更が起きたことを Sequencer に伝える(トラックが再描画される）
	GetSequencer()->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
}

FReply FMovieSceneConvTrackEditor::OnCreateButtonClicked(UMovieSceneTrack* track) {

	const FScopedTransaction Transaction(LOCTEXT("AddAkAudioEvent_Transaction", "Add AkAudioEvent"));

	track->Modify();
#if UE_4_20_OR_LATER
	FFrameNumber KeyTime = GetSequencer()->GetGlobalTime().Time.FrameNumber;
#else
	float KeyTime = GetSequencer()->GetGlobalTime();
#endif
	auto ConvTrack = Cast<UMovieSceneConversationTrack>(track);
	ConvTrack->AddNewItem(KeyTime);

	GetSequencer()->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);

	return FReply::Handled();
}

TSharedRef<SWidget> FMovieSceneConvTrackEditor::MakeAddButton(FText HoverText, FOnClicked OnClicked, const TAttribute<bool>& HoverState)
{
	FSlateFontInfo SmallLayoutFont = FCoreStyle::GetDefaultFontStyle("Regular", 8);

	TSharedRef<STextBlock> ButtonText = SNew(STextBlock)
		.Text(HoverText)
		.Font(SmallLayoutFont)
		.ColorAndOpacity(FSlateColor::UseForeground());

	TSharedRef<SButton> Button =

		SNew(SButton)
		.ButtonStyle(FEditorStyle::Get(), "HoverHintOnly")
		.ForegroundColor(FSlateColor::UseForeground())
		.OnClicked(OnClicked)
		.ContentPadding(FMargin(5, 2))
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		.Padding(FMargin(0, 0, 2, 0))
		[
			SNew(SImage)
			.ColorAndOpacity(FSlateColor::UseForeground())
		.Image(FEditorStyle::GetBrush("Plus"))
		]

	+ SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		.AutoWidth()
		[
			ButtonText
		]
		];

	//TAttribute<EVisibility> Visibility = TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateStatic(GetRolloverVisibility, HoverState, TWeakPtr<SButton>(ComboButton)));
	//ComboButtonText->SetVisibility(Visibility);

	return Button;
}

#undef LOCTEXT_NAMESPACE