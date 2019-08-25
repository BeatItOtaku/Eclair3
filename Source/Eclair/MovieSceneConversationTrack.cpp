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

	TimeUnit endTime = Time + 1.0f;

	for (auto s : GetAllSections()) {
		if (s->IsTimeWithinSection(Time)) {
			endTime = s->GetEndTime();
			s->SetEndTime(Time);
		}
	}

#if UE_4_20_OR_LATER
	NewSection->InitialPlacement(GetAllSections(), Time, 1.0f, SupportsMultipleRows());
#else
	NewSection->InitialPlacement(GetAllSections(), Time, endTime, SupportsMultipleRows());
#endif

	AddSection(*NewSection);

	RemapSectionsItem();

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