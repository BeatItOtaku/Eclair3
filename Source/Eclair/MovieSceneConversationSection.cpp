#include "MovieSceneConversationSection.h"

void  UMovieSceneConversationSection::BeginDestroy()
{

}

FMovieSceneEvalTemplatePtr UMovieSceneConversationSection::GenerateTemplate() const
{
	return FMovieSceneConversationTemplate(this);
}