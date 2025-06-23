// Optics Team

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	None,
	Warrior,
	Mage,
	Rogue,
	Cleric,
	Paladin,
	Ranger,
	Druid
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TSubclassOf<class UGameplayEffect> PrimaryAttributes;
};

/**
 * 
 */
UCLASS()
class SURVIVORS_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

	public:
		UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
		TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;

		UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
		TSubclassOf<class UGameplayEffect> SecondaryAttributes;

		UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
		TSubclassOf<class UGameplayEffect> VitalAttributes;

		FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);
	
};
