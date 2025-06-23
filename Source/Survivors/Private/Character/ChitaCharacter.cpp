

// Optics Team
#include "Character/ChitaCharacter.h"
#include "AbilitySystemComponent.h"
#include <Character/ChitaPlayerState.h>

AChitaCharacter::AChitaCharacter()
{
}

void AChitaCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
	
}

void AChitaCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}

void AChitaCharacter::InitAbilityActorInfo()
{
	AChitaPlayerState* ChitaPlayerState = GetPlayerState<AChitaPlayerState>();
	//check(ChitaPlayerState);
	if (ChitaPlayerState) {
		ChitaPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(ChitaPlayerState, this);
		AbilitySystemComponent = ChitaPlayerState->GetAbilitySystemComponent();
		AttributeSet = ChitaPlayerState->GetAttributeSet();
	}
	if (AbilitySystemComponent && DefaultAttributeEffect) {
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);
		if (NewHandle.IsValid()) {
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
		}
	}
	InitializeDefaultAttributes();
	GiveDefaultAbilities();
}

void AChitaCharacter::GiveDefaultAbilities()
{
	if (AbilitySystemComponent && DefaultAbilities.Num() > 0) {
		for (TSubclassOf<UGameplayAbility>& Ability : DefaultAbilities) {
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability.GetDefaultObject(), 1, 0));
		}
	}
}
