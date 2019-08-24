// Copyright (c) 2006-2016 Audiokinetic Inc. / All Rights Reserved

#include "MovieSceneConversationTrack.h"

#include "MovieSceneConversationSection.h"
#include "IMovieScenePlayer.h"
#include "MovieScene.h"

FMovieSceneEvalTemplatePtr UMovieSceneConversationTrack::CreateTemplateForSection(const UMovieSceneSection& InSection) const
{
	return InSection.GenerateTemplate();
}

UMovieSceneSection* UMovieSceneConversationTrack::CreateNewSection()
{
	return NewObject<UMovieSceneSection>(this, UMovieSceneConversationSection::StaticClass(), NAME_None, RF_Transactional);
}

bool UMovieSceneConversationTrack::AddNewEvent(TimeUnit Time, UEclairConversation* Event, const FString& EventName)
{
	if (Event == nullptr && EventName.IsEmpty())
		return false;

	UMovieSceneConversationSection* NewSection = NewObject<UMovieSceneConversationSection>(this);
	ensure(NewSection);

#if UE_4_20_OR_LATER
	NewSection->InitialPlacement(GetAllSections(), Time, Duration, SupportsMultipleRows());
#else
	NewSection->InitialPlacement(GetAllSections(), Time, Time + 10.0f/*+ Duration*/, SupportsMultipleRows());
#endif
	AddSection(*NewSection);

	return true;
}

FName UMovieSceneConversationTrack::GetTrackName() const
{
	static FName TrackName("AkAudioEvents");
	return TrackName;
}