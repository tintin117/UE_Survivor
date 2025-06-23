// Optics Team


#include "Character/ChitaPlayerState.h"
#include <GAS/ChitaAbilitySystemComponent.h>
#include "GAS/ChitaAttributeSet.h"

AChitaPlayerState::AChitaPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UChitaAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UChitaAttributeSet>(TEXT("AttributeSet"));

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetCurrentHealthAttribute()).AddUObject(this, &AChitaPlayerState::HandleHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddUObject(this, &AChitaPlayerState::HandleMaxHealthChanged);

	NetUpdateFrequency = 100.0f;
}

UAbilitySystemComponent* AChitaPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AChitaPlayerState::BeginPlay()
{
	Super::BeginPlay();

	// Restore attributes if transitioning from another level
	//AttributeSet->RestoreAttributes();
}

void AChitaPlayerState::HandleHealthChanged(const FOnAttributeChangeData& Data)
{
	float DeltaValue = Data.NewValue - Data.OldValue;
	FGameplayTagContainer EventTags;
	OnHealthChanged(Data.NewValue, EventTags);
}

void AChitaPlayerState::HandleMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	float DeltaValue = Data.NewValue - Data.OldValue;
	FGameplayTagContainer EventTags;
	OnMaxHealthChanged(Data.NewValue, EventTags);
}
