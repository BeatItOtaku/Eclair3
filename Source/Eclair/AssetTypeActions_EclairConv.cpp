#include "AssetTypeActions_EclairConv.h"
#include "Eclair.h"
#include "EclairConversation.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

UClass* FAssetTypeActions_EclairConv::GetSupportedClass() const
{
	return UEclairConversation::StaticClass();
}

#undef LOCTEXT_NAMESPACE