#include "MovieSceneConversationTrack.h"

#include "MovieSceneConversationSection.h"
#include "IMovieScenePlayer.h"
#include "MovieScene.h"
#include "EclairConversation.h"
#include "MovieSceneConversationSection.h"

FMovieSceneEvalTemplatePtr UMovieSceneConversationTrack::CreateTemplateForSection(const UMovieSceneSection& InSection) const
{
	return InSection.GenerateTemplate();
}

UMovieSceneSection* UMovieSceneConversationTrack::CreateNewSection()
{
	return NewObject<UMovieSceneSection>(this, UMovieSceneConversationSection::StaticClass(), NAME_None, RF_Transactional);
}

bool UMovieSceneConversationTrack::AddNewItem(TimeUnit Time)
{
	UMovieSceneConversationSection* NewSection = NewObject<UMovieSceneConversationSection>(this);
	ensure(NewSection);

#if UE_4_20_OR_LATER
	NewSection->InitialPlacement(GetAllSections(), Time, Duration, SupportsMultipleRows());
#else
	NewSection->InitialPlacement(GetAllSections(), Time, Time + 10.0f/*+ Duration*/, SupportsMultipleRows());
#endif
	//NewSection->Item = Conversation->Get(0);
	AddSection(*NewSection);

	volatile auto hoge = GetAllSections();

	return true;
}

FName UMovieSceneConversationTrack::GetTrackName() const
{
	static FName TrackName("Conversation");
	return TrackName;
}

void UMovieSceneConversationTrack::RemapSectionsItem()
{
	if (Conversation == nullptr) return;
	for (int i = 0; i < Sections.Num(); i++) {
		CastChecked<UMovieSceneConversationSection>(Sections[i])->Item = (Conversation->Items.Num() > i) ? Conversation->Items[i] : FEclairConversationItem();
	}
}