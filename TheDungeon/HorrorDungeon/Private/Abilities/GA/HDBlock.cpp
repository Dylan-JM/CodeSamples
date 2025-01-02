// Copyright DM


#include "Abilities/GA/HDBlock.h"

#include "Abilities/HDAttributeSet.h"
#include "PlayerCharacter/BaseCharacter.h"

void UHDBlock::SetBlockChance(float InBlockChance)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (UHDAttributeSet* HDAS = Cast<UHDAttributeSet>(Character->GetAttributeSet()))
		{
			HDAS->SetBlockChance(InBlockChance);
		}
	}
}
