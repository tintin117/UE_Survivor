// Optics Team

#pragma once

#include "CoreMinimal.h"
#include "Character/ChitaCharacterBase.h"
//#include "GAS/ChitaAttributeSet.h"
//#include "StateTree.h"
#include <GameplayEffect.h>
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVORS_API AEnemyCharacter : public AChitaCharacterBase
{
	GENERATED_BODY()
public:
	AEnemyCharacter();
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	//TObjectPtr<UStateTree> AssignedStateTree;
	//virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//UChitaAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
	void OnHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags);
	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
	void OnMaxHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags);
	virtual void GiveDefaultAbilities();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "GAS")
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass, FGameplayTag DataTag, float Magnitude);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilities;

private:
	void InitAbilityActorInfo();
	//Function to handle changes in health attribute.
	void HandleHealthChanged(const FOnAttributeChangeData& Data);
	void HandleMaxHealthChanged(const FOnAttributeChangeData& Data);
};
