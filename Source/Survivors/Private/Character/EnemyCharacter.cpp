// Optics Team


#include "Character/EnemyCharacter.h"
#include "GAS/ChitaAbilitySystemComponent.h"
#include "GAS/ChitaAttributeSet.h"

AEnemyCharacter::AEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UChitaAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UChitaAttributeSet>("AttributeSet");

	
}

void AEnemyCharacter::BeginPlay()
{

	Super::BeginPlay();
	

	UChitaAttributeSet* ChitaAS = Cast<UChitaAttributeSet>(AttributeSet);
	if (ChitaAS)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ChitaAS->GetCurrentHealthAttribute()).AddUObject(this, &AEnemyCharacter::HandleHealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ChitaAS->GetMaxHealthAttribute()).AddUObject(this, &AEnemyCharacter::HandleMaxHealthChanged);

	}
	InitAbilityActorInfo();

	
}

void AEnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeDefaultAttributes();
	
}
void AEnemyCharacter::HandleHealthChanged(const FOnAttributeChangeData& Data)
{
	float DeltaValue = Data.NewValue - Data.OldValue;
	FGameplayTagContainer EventTags;
	OnHealthChanged(Data.NewValue, EventTags);
}

void AEnemyCharacter::HandleMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	float DeltaValue = Data.NewValue - Data.OldValue;
	FGameplayTagContainer EventTags;
	OnMaxHealthChanged(Data.NewValue, EventTags);
}