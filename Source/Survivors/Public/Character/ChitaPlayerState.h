// Optics Team

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"

#include "AbilitySystemComponent.h"
#include "GAS/ChitaAttributeSet.h"
#include "ChitaPlayerState.generated.h"

UCLASS()
class SURVIVORS_API AChitaPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AChitaPlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	void BeginPlay();
	UChitaAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
	void OnHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags);
	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
	void OnMaxHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	//UPROPERTY(Replicated)
	UAbilitySystemComponent* AbilitySystemComponent;

	// Attribute Set that stores and manages health and other attributes, marked for replication.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	//UPROPERTY(Replicated)
	UChitaAttributeSet* AttributeSet;

private:
	//Function to handle changes in health attribute.
	void HandleHealthChanged(const FOnAttributeChangeData& Data);
	void HandleMaxHealthChanged(const FOnAttributeChangeData& Data);
};
