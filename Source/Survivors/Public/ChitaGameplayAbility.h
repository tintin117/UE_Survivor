// Optics Team

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ChitaGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVORS_API UChitaGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

    // Override to check if the ability has enough cost (Focus/HP/etc.)
    virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        OUT FGameplayTagContainer* OptionalRelevantTags) const override;

    // Override to apply the cost gameplay effects
    virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo) const override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
    UDataTable* DataTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    FName RowName;

    // Editable array of cost GEs
    UPROPERTY(EditDefaultsOnly, Category = "Ability|Cost")
    TArray<TSubclassOf<class UGameplayEffect>> CostGameplayEffectClasses;

    // Optional: total cost required (in focus-equivalent units)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Cost")
    float TotalAbilityCost;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
    float Damage;

    UFUNCTION(BlueprintCallable, Category = "Ability|Config")
    void LoadRowFromBlueprintDataTable();

    void LogRowFields(const uint8* RowData, const UScriptStruct* RowStruct);
};
