// Optics Team


#include "Character/ChitaCharacterBase.h"
#include <SurvivorGameInstance.h>


// Sets default values
AChitaCharacterBase::AChitaCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

}

UAbilitySystemComponent* AChitaCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AChitaCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AChitaCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level)
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	const FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, EffectContext);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void AChitaCharacterBase::InitializeDefaultAttributes()
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	//ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	USurvivorGameInstance* GI = Cast<USurvivorGameInstance>(GetGameInstance());
	if (GI)
	{
		//UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
		//FGameplayAttribute CurrentHealth = 
		//GetAbilitySystemComponent()->SetNumericAttributeBase(AttributeSet->GetCurrentHealthAttribute(), GI->SavedHealth);
	}
}

