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

bool UMovieSceneConversationTrack::AddNewItem(TimeUnit Time,TimeUnit Duration)
{
	UMovieSceneConversationSection* NewSection = NewObject<UMovieSceneConversationSection>(this);
	ensure(NewSection);

	TimeUnit endTime = Time + Duration;

	TArray<UMovieSceneSection*> sections = GetAllSections();

	//セクションの途中なら分割する
	for (auto s : sections) {
		if (Time > s->GetStartTime() && Time < s->GetEndTime()) {
			endTime = s->GetEndTime();
			s->SetEndTime(Time);
		}
		else if (Time == s->GetStartTime()) {
			endTime = (s->GetStartTime() + s->GetEndTime()) / 2;
			s->SetStartTime(endTime);
		}
	}
	//もしこれから置かれるセクションが最後になるなら現在の最後を伸ばす(空白をなくす)
	if(!IsEmpty()) if (sections[sections.Num() - 1]->GetEndTime() < Time) {
		sections[sections.Num() - 1]->SetEndTime(Time);
	}

#if UE_4_20_OR_LATER
	NewSection->InitialPlacement(GetAllSections(), Time, endTime - Time, SupportsMultipleRows());
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