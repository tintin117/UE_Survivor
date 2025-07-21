// Optics Team


#include "GAS/GASActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"

// Sets default values
AGASActor::AGASActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot")));
}

// Called when the game starts or when spawned
void AGASActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGASActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass, FGameplayTag DataTag, float Magnitude)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr)
	{
		return;
	}

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1, EffectContextHandle);
	if (EffectSpecHandle.IsValid())
	{
		FGameplayEffectSpec* Spec = EffectSpecHandle.Data.Get();
		Spec->SetSetByCallerMagnitude(DataTag, Magnitude);
		TargetASC->ApplyGameplayEffectSpecToSelf(*Spec);
	}
}

void AGASActor::ApplyGameplayEffectSpecToTarget(FGameplayEffectSpecHandle SpecHandle, AActor* TargetActor)  
{  
   UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);  
   if (TargetASC == nullptr || !SpecHandle.IsValid())  
   {  
       return;  
   }  

   const FGameplayEffectSpec* Spec = SpecHandle.Data.Get();  
   if (Spec)  
   {  
       TargetASC->ApplyGameplayEffectSpecToSelf(*Spec);  
   }  
}

// Called every frame
void AGASActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

