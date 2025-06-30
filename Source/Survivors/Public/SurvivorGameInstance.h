// Optics Team

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SurvivorGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVORS_API USurvivorGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "Saved Data")
	float SavedHealth = 0.f;
};
