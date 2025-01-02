// Copyright DM


#include "Abilities/Data/AbilityInfo.h"

#include "HorrorDungeon/HDLogChannels.h"

FHDAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	if (!AbilityTag.IsValid()) return FHDAbilityInfo();
	for (const FHDAbilityInfo Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogHD, Error, TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}

	return FHDAbilityInfo();
}
