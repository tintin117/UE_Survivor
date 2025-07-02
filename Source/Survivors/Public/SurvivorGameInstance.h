// Optics Team

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Containers/Map.h" 
#include "GAS/ChitaAttributeSet.h"
#include "SurvivorGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVORS_API USurvivorGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Saved Data")
	TMap<FGameplayAttribute, float> SavedAttributes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Saved Data")
	bool bIsInRoom = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Saved Data")
	int CurrentWave = 1;
};
