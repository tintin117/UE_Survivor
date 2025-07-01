// Optics Team

#pragma once

#include "CoreMinimal.h"
#include "Character/ChitaCharacterBase.h"
#include <SurvivorGameInstance.h>
#include "ChitaCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVORS_API AChitaCharacter : public AChitaCharacterBase
{
	GENERATED_BODY()

public:
	AChitaCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void GiveDefaultAbilities();
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilities;
	
private:
	void InitAbilityActorInfo();
	void ApplySaveData();
};
