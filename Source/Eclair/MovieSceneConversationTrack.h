#pragma once

#include "MovieScene.h"
#include "MovieSceneNameableTrack.h"
#include "IMovieSceneTrackInstance.h"
#include "EclairUEFeatures.h"
#include "EclairConversation.h"
#include "MovieSceneConversationTrack.generated.h"

class UMovieSceneConversationSection;

UCLASS( MinimalAPI )
class UMovieSceneConversationTrack : public UMovieSceneNameableTrack
{
	GENERATED_BODY()

public:
	UMovieSceneConversationTrack()
	{
#if WITH_EDITORONLY_DATA
		SetColorTint(FColor(216,71,0,255));
#endif
	}

	virtual void RemoveAllAnimationData() override { Sections.Empty(); }
	
	virtual bool HasSection(const UMovieSceneSection& Section) const override { return Sections.Contains(&Section); }
	virtual void AddSection(UMovieSceneSection& Section) override {
		Sections.Add(&Section);
		Sections.Sort([](const UMovieSceneSection& A, const UMovieSceneSection& B) {
			return A.GetStartTime() < B.GetStartTime();
		});
	}
	virtual void RemoveSection(UMovieSceneSection& Section) override { Sections.Remove(&Section); }
	virtual bool IsEmpty() const override { return Sections.Num() == 0; }
	virtual const TArray<UMovieSceneSection*>& GetAllSections() const override { return Sections; }

#if !UE_4_20_OR_LATER
	virtual TRange<float> GetSectionBoundaries() const override
	{
		TArray< TRange<float> > Bounds;

		for (int32 SectionIndex = 0; SectionIndex < Sections.Num(); ++SectionIndex)
		{
			Bounds.Add(Sections[SectionIndex]->GetRange());
		}

		return TRange<float>::Hull(Bounds);
	}
#endif
	/** end UMovieSceneTrack interface */

#if UE_4_20_OR_LATER
	typedef FFrameNumber TimeUnit;
#else
	typedef float TimeUnit;
#endif


	ECLAIR_API virtual UMovieSceneSection* CreateNewSection() override;
	virtual bool SupportsMultipleRows() const override { return false; }

	ECLAIR_API virtual FName GetTrackName() const override;

	ECLAIR_API bool AddNewItem(TimeUnit Time, TimeUnit Duration = 1.0f);

	UPROPERTY(EditAnywhere, Category = "ConversationTrack")
		UEclairConversation* Conversation;

	ECLAIR_API void RemapSectionsItem();

protected:

	/** All the sections in this track */
	UPROPERTY()
		TArray<UMovieSceneSection*> Sections;

	ECLAIR_API virtual FMovieSceneEvalTemplatePtr CreateTemplateForSection(const UMovieSceneSection& InSection) const override;
};
