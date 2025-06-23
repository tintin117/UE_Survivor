// Optics Team

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ChitaAttributeSet.generated.h"

/**
 * 
 */
//Macro to easily declare attributes
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class SURVIVORS_API UChitaAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	//Constructor and overrides
	UChitaAttributeSet();

	/** Primary Attributes */
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes", ReplicatedUsing=OnRep_CurrentHealth)
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UChitaAttributeSet, CurrentHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UChitaAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes", ReplicatedUsing = OnRep_Focus)
	FGameplayAttributeData Focus;
	ATTRIBUTE_ACCESSORS(UChitaAttributeSet, Focus)

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes", ReplicatedUsing = OnRep_MaxFocus)
	FGameplayAttributeData MaxFocus;
	ATTRIBUTE_ACCESSORS(UChitaAttributeSet, MaxFocus)

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes", ReplicatedUsing = OnRep_AffinityMod)
	FGameplayAttributeData AffinityMod;
	ATTRIBUTE_ACCESSORS(UChitaAttributeSet, AffinityMod)

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes", ReplicatedUsing = OnRep_CritChance)
	FGameplayAttributeData CritChance;
	ATTRIBUTE_ACCESSORS(UChitaAttributeSet, CritChance)

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes", ReplicatedUsing = OnRep_CritMod)
	FGameplayAttributeData CritMod;
	ATTRIBUTE_ACCESSORS(UChitaAttributeSet, CritMod)

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes", ReplicatedUsing = OnRep_Armor)
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UChitaAttributeSet, Armor)

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes", ReplicatedUsing = OnRep_Shield)
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UChitaAttributeSet, Shield)

	/*
	 * Meta Attributes
	 */

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UChitaAttributeSet, IncomingDamage)
	

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void SaveAttributes();
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void RestoreAttributes();


protected:
	//RepNotify functions
	UFUNCTION()
	virtual void OnRep_CurrentHealth(const FGameplayAttributeData& OldHealth);
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Focus(const FGameplayAttributeData& OldFocus);

	UFUNCTION()
	virtual void OnRep_MaxFocus(const FGameplayAttributeData& OldMaxFocus);

	UFUNCTION()
	virtual void OnRep_AffinityMod(const FGameplayAttributeData& OldAffinityMod);

	UFUNCTION()
	virtual void OnRep_CritChance(const FGameplayAttributeData& OldCritChance);

	UFUNCTION()
	virtual void OnRep_CritMod(const FGameplayAttributeData& OldCritMod);

	UFUNCTION()
	virtual void OnRep_Armor(const FGameplayAttributeData& OldArmor);

	UFUNCTION()
	virtual void OnRep_Shield(const FGameplayAttributeData& OldShield);

	/**Attribute change handling*/
	// Function to handle when any attribute changes, allowing clamping or validation
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/**Gameplay effect execution handling*/
	// Function called after a gameplay effect modifies an attribute to handle post-modification logic.
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

private:
	float SavedHealth;
	float SavedMaxHealth;
};
