#include "MovieSceneConversationSection.h"

FMovieSceneEvalTemplatePtr UMovieSceneConversationSection::GenerateTemplate() const
{
	return FMovieSceneConversationTemplate(this);
}