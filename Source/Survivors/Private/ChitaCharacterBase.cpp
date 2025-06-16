// Optics Team


#include "ChitaCharacterBase.h"

// Sets default values
AChitaCharacterBase::AChitaCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChitaCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChitaCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChitaCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

