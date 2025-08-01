// Optics Team


#include "Character/EnemyCharacter.h"
#include "GAS/ChitaAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
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
	GiveDefaultAbilities();
	
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
void AEnemyCharacter::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass, FGameplayTag DataTag, float Magnitude)  
{  
   UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);  
   if (TargetASC == nullptr)  
   {  
       return;  
   }  

   check(GameplayEffectClass);  
   FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();  
   EffectContextHandle.AddSourceObject(this);  
   FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1, EffectContextHandle);  
   if (EffectSpecHandle.IsValid())  
   {  
       FGameplayEffectSpec* Spec = EffectSpecHandle.Data.Get();  
       Spec->SetSetByCallerMagnitude(DataTag, Magnitude);  
       TargetASC->ApplyGameplayEffectSpecToSelf(*Spec);  
   }  
}
void AEnemyCharacter::GiveDefaultAbilities()
{
	if (AbilitySystemComponent && DefaultAbilities.Num() > 0) {
		for (TSubclassOf<UGameplayAbility>& Ability : DefaultAbilities) {
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability.GetDefaultObject(), 1, 0));
		}
	}
}