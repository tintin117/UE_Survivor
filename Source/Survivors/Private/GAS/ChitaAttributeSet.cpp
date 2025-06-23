// Optics Team


#include "GAS/ChitaAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

UChitaAttributeSet::UChitaAttributeSet()
{
	// Set default attribute values
	InitCurrentHealth(100.f);
	InitMaxHealth(100.f);
	InitFocus(0);
	InitMaxFocus(90.f);
	InitArmor(0.f);
}

void UChitaAttributeSet::SaveAttributes()
{
	SavedHealth = CurrentHealth.GetBaseValue();
	SavedMaxHealth = MaxHealth.GetBaseValue();
}

void UChitaAttributeSet::RestoreAttributes()
{
	InitCurrentHealth(SavedHealth);
	InitMaxHealth(SavedMaxHealth);
}

//Called on clients when CurrentHealth is updated, to synchronize the new value.
void UChitaAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UChitaAttributeSet, CurrentHealth, OldHealth);
}

//Called on clients when MaxHealth is updated, to synchronize the new value.
void UChitaAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UChitaAttributeSet, MaxHealth, OldMaxHealth);
}

//Called on clients when Focus is updated, to synchronize the new value.
void UChitaAttributeSet::OnRep_Focus(const FGameplayAttributeData& OldFocus)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UChitaAttributeSet, Focus, OldFocus);
}

void UChitaAttributeSet::OnRep_MaxFocus(const FGameplayAttributeData& OldMaxFocus)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UChitaAttributeSet, MaxFocus, OldMaxFocus);
}

void UChitaAttributeSet::OnRep_AffinityMod(const FGameplayAttributeData& OldAffinityMod)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UChitaAttributeSet, AffinityMod, OldAffinityMod);
}

void UChitaAttributeSet::OnRep_CritChance(const FGameplayAttributeData& OldCritChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UChitaAttributeSet, CritChance, OldCritChance);
}

void UChitaAttributeSet::OnRep_CritMod(const FGameplayAttributeData& OldCritMod)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UChitaAttributeSet, CritMod, OldCritMod);
}

void UChitaAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UChitaAttributeSet, Armor, OldArmor);
}

void UChitaAttributeSet::OnRep_Shield(const FGameplayAttributeData& OldShield)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UChitaAttributeSet, Shield, OldShield);
}

// Runs before any attribute value changes, to handle validation or clamping.
void UChitaAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetCurrentHealthAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, MaxHealth.GetCurrentValue());
	}
	/*if (Attribute == GetFocusAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, MaxFocus.GetCurrentValue());
	}*/
}

// Handles additional logic after a gameplay effect has been executed.
void UChitaAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data); 
	if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		SetCurrentHealth(FMath::Clamp<float>(GetCurrentHealth(), 0.0f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetFocusAttribute())
	{
		float Clamped = FMath::Clamp(Focus.GetCurrentValue(), 0.0f, MaxFocus.GetCurrentValue());
		//Focus.SetCurrentValue(Clamped);
		SetFocus(Clamped);
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.0f); // Reset incoming damage after processing
		if (LocalIncomingDamage > 0.0f)
		{
			float NewHealth = GetCurrentHealth() - LocalIncomingDamage;
			SetCurrentHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
			if (NewHealth <= 0.0f)
			{
				// Handle death logic here if needed
			}
		}
		else
		{
			SetCurrentHealth(FMath::Clamp(GetCurrentHealth(), 0.0f, GetMaxHealth()));
		}
	}
}

// Defines which properties are replicated to clients over the network.
void UChitaAttributeSet::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UChitaAttributeSet, CurrentHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UChitaAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UChitaAttributeSet, Focus, COND_None, REPNOTIFY_Always);
}
