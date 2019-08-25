#pragma once

#include "MovieSceneSection.h"
#include "Evaluation/MovieSceneEvalTemplate.h"
#include "EclairConversationItem.h"

#include "MovieSceneConversationSection.generated.h"

/**
* A single floating point section that triggers a Wwise event.
*/
UCLASS(MinimalAPI)
class UMovieSceneConversationSection : public UMovieSceneSection
{
	GENERATED_BODY()

public:

	void BeginDestroy() override;

	/* UMovieSceneSection interface */
	ECLAIR_API virtual FMovieSceneEvalTemplatePtr GenerateTemplate() const override;

#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& e) override {};
#endif//WITH_EDITOR

public:
	UPROPERTY(EditAnywhere, Category = "ConversationItem")
		bool isSectionHead = false;

	FEclairConversationItem Item = FEclairConversationItem();

};

USTRUCT()
struct ECLAIR_API FMovieSceneConversationTemplate
	: public FMovieSceneEvalTemplate
{
	GENERATED_BODY()

		FMovieSceneConversationTemplate() {}

	FMovieSceneConversationTemplate(const UMovieSceneConversationSection* InSection) {}

	virtual void Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const override {}

	virtual UScriptStruct& GetScriptStructImpl() const override { return *StaticStruct(); }

	virtual void Setup(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const override {}
	virtual void TearDown(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const override {}
	virtual void SetupOverrides() override { EnableOverrides(RequiresSetupFlag | RequiresTearDownFlag); }

	UPROPERTY()
		const UMovieSceneConversationSection* Section = nullptr;
};