// Optics Team


#include "GAS/ExecCalc/ExecCalc_Damage.h"
#include "GAS/ChitaAttributeSet.h"
#include "AbilitySystemComponent.h"

struct ChitaDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritMod);
	ChitaDamageStatics() 
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UChitaAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UChitaAttributeSet, CritChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UChitaAttributeSet, CritMod, Source, false);
	}
};

static const ChitaDamageStatics& DamageStatics()
{
	static ChitaDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritModDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters Evaluationparameters;
	Evaluationparameters.SourceTags = SourceTags;
	Evaluationparameters.TargetTags = TargetTags;

	float Damage = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")));

	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, Evaluationparameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.0f);

	float SourceCritChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritChanceDef, Evaluationparameters, SourceCritChance);
	SourceCritChance = FMath::Max<float>(SourceCritChance, 0.0f);

	float SourceCritMod = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritModDef, Evaluationparameters, SourceCritMod);
	SourceCritMod = FMath::Max<float>(SourceCritMod, 0.0f);


	const bool bIsCrit = FMath::RandRange(0, 1) <= SourceCritChance;
	Damage = bIsCrit ? Damage * SourceCritMod : Damage;

	Damage = Damage * (1 - TargetArmor);

	const FGameplayModifierEvaluatedData EvaluatedData(UChitaAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
