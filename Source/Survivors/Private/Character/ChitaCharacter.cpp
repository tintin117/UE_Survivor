

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
  ApplySaveData();
}

void AChitaCharacter::ApplySaveData()
{
	USurvivorGameInstance* GI = Cast<USurvivorGameInstance>(GetGameInstance());
	if (GI->bIsInRoom)
	{
		UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
		for (const TPair<FGameplayAttribute, float>& Pair : GI->SavedAttributes) {
			ASC->SetNumericAttributeBase(Pair.Key, Pair.Value);
		}
		//ASC->SetNumericAttributeBase(UChitaAttributeSet::GetCurrentHealthAttribute(), GI->SavedHealth);
	}
}

void AChitaCharacter::GiveDefaultAbilities()
{
	if (AbilitySystemComponent && DefaultAbilities.Num() > 0) {
		for (TSubclassOf<UGameplayAbility>& Ability : DefaultAbilities) {
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability.GetDefaultObject(), 1, 0));
		}
	}
}
