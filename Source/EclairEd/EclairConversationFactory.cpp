// Fill out your copyright notice in the Description page of Project Settings.

#include "EclairConversationFactory.h"
#include "EclairConversation.h"
#include "EclairEd.h"

#if PLATFORM_WINDOWS
#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include "cereal-UE4.hxx"
#include <sstream>
#endif

/*UEclairConversationFactory::UEclairConversationFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UEclairConversation::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}*/
UEclairConversationFactory::UEclairConversationFactory()
	: Super()
{
	SupportedClass = UEclairConversation::StaticClass();
	bCreateNew = false;
	bEditAfterNew = true;
	bEditorImport = true;
	bText = true;
	Formats.Add(TEXT("eclairconversation;Eclair Conversation"));
}
bool UEclairConversationFactory::DoesSupportClass(UClass* Class)
{
	return (Class == UEclairConversation::StaticClass());
}
UClass* UEclairConversationFactory::ResolveSupportedClass()
{
	return UEclairConversation::StaticClass();
}
UObject* UEclairConversationFactory::FactoryCreateNew(
	UClass* InClass,
	UObject* InParent,
	FName InName,
	EObjectFlags Flags,
	UObject* Context,
	FFeedbackContext* Warn
)
{
	UEclairConversation* NewEclairConversation = NewObject<UEclairConversation>(InParent, InClass, InName, Flags);
	return NewEclairConversation;
}

UObject* UEclairConversationFactory::FactoryCreateText(
	UClass* InClass,
	UObject* InParent,
	FName InName,
	EObjectFlags Flags,
	UObject* Context,
	const TCHAR* Type,
	const TCHAR*& Buffer,
	const TCHAR* BuferEnd,
	FFeedbackContext* Warn
)
{
	TArray<FString> Values;
	FString(Buffer).ParseIntoArray(Values, TEXT(","), true);

#if PLATFORM_WINDOWS
	std::stringstream stream;
	stream << TCHAR_TO_UTF8(Buffer);
	cereal::JSONInputArchive archive(stream);
#endif

	UEclairConversation* NewEclairConversation = NewObject<UEclairConversation>(InParent, InClass, InName, Flags);

	if (NewEclairConversation)
	{
		/*archive(
			cereal::make_nvp("description", NewEclairConversation->Description),
			cereal::make_nvp("type", NewEclairConversation->Type),
			cereal::make_nvp("character-left", NewEclairConversation->CharacterLeft),
			cereal::make_nvp("character-right", NewEclairConversation->CharacterRight),
			cereal::make_nvp("content", NewEclairConversation->Items)
		);*/
#if PLATFORM_WINDOWS
		NewEclairConversation->serialize(archive);
#endif

		if (!NewEclairConversation->AssetImportData)
		{
			NewEclairConversation->AssetImportData = NewObject<UAssetImportData>(NewEclairConversation, UAssetImportData::StaticClass());
				//ConstructObject<UAssetImportData>(UAssetImportData::StaticClass(), NewEclairConversation);
		}

		NewEclairConversation->AssetImportData->Update(CurrentFilename);
	}

	return NewEclairConversation;
}

bool UEclairConversationFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
{
	UEclairConversation* EclairConversation = Cast<UEclairConversation>(Obj);
	if (EclairConversation && EclairConversation->AssetImportData)
	{
		EclairConversation->AssetImportData->ExtractFilenames(OutFilenames);
		return true;
	}
	return false;
}
void UEclairConversationFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths)
{
	UEclairConversation* EclairConversation = Cast<UEclairConversation>(Obj);
	if (EclairConversation && ensure(NewReimportPaths.Num() == 1))
	{
		EclairConversation->AssetImportData->UpdateFilenameOnly(NewReimportPaths[0]);
	}
}
EReimportResult::Type UEclairConversationFactory::Reimport(UObject* Obj)
{
    #if PLATFORM_MAC
        return EReimportResult::Failed;
    #endif
    
	UEclairConversation* EclairConversation = Cast<UEclairConversation>(Obj);
	if (!EclairConversation)
	{
		return EReimportResult::Failed;
	}

	const FString Filename = IFileManager::Get().ConvertToRelativePath(*(EclairConversation->AssetImportData->GetFirstFilename()));
	if (!Filename.Len() || IFileManager::Get().FileSize(*Filename) == INDEX_NONE)
	{
		return EReimportResult::Failed;
	}

	EReimportResult::Type Result = EReimportResult::Failed;
	if (UFactory::StaticImportObject(
		EclairConversation->GetClass(), EclairConversation->GetOuter(),
		*EclairConversation->GetName(), RF_Public | RF_Standalone, *Filename, NULL, this))
	{
		if (EclairConversation->GetOuter())
		{
			EclairConversation->GetOuter()->MarkPackageDirty();
		}
		else
		{
			EclairConversation->MarkPackageDirty();
		}
		return EReimportResult::Succeeded;
	}

	return EReimportResult::Failed;
}

