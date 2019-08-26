#include "MovieSceneConversationSection.h"

void  UMovieSceneConversationSection::BeginDestroy()
{
	Super::BeginDestroy();
}

FMovieSceneEvalTemplatePtr UMovieSceneConversationSection::GenerateTemplate() const
{
	return FMovieSceneConversationTemplate(this);
}