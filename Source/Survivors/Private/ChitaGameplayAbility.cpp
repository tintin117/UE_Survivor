// Optics Team


#include "ChitaGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GAS/ChitaAttributeSet.h"
#include "AbilitySystemGlobals.h"


void UChitaGameplayAbility::LoadRowFromBlueprintDataTable()
{
    if (!DataTable || RowName.IsNone())
        return;

    // Get the struct type used in the DataTable
    const UScriptStruct* RowStruct = DataTable->GetRowStruct();
    if (!RowStruct)
        return;

    // Get the actual row
    const uint8* RowData = DataTable->FindRowUnchecked(RowName);
    if (!RowData)
        return;

    // Example: log all property names and values
    for (TFieldIterator<FProperty> PropIt(RowStruct); PropIt; ++PropIt)
    {
        FProperty* Prop = *PropIt;
        FString PropName = Prop->GetName();
        FString ValueStr;
        Prop->ExportText_Direct(ValueStr, Prop->ContainerPtrToValuePtr<void>(RowData), nullptr, nullptr, 0);
        UE_LOG(LogTemp, Log, TEXT("%s = %s"), *PropName, *ValueStr);
    }
}

void UChitaGameplayAbility::LogRowFields(const uint8* RowData, const UScriptStruct* RowStruct)
{
    for (TFieldIterator<FProperty> PropIt(RowStruct); PropIt; ++PropIt)
    {
        const FProperty* Prop = *PropIt;
        if (!Prop) continue;

        FString PropName = Prop->GetName();
        FString Value;
        Prop->ExportText_Direct(Value, Prop->ContainerPtrToValuePtr<void>(RowData), nullptr, nullptr, 0);
        UE_LOG(LogTemp, Log, TEXT("  %s = %s"), *PropName, *Value);
    }
}


#if WITH_EDITOR
void UChitaGameplayAbility::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    const FName ChangedProp = PropertyChangedEvent.Property
        ? PropertyChangedEvent.Property->GetFName()
        : NAME_None;

    if (ChangedProp == GET_MEMBER_NAME_CHECKED(UChitaGameplayAbility, DataTable) ||
        ChangedProp == GET_MEMBER_NAME_CHECKED(UChitaGameplayAbility, RowName))
    {
        LoadRowFromBlueprintDataTable();
        {
            TotalAbilityCost = 0.f;

            if (DataTable && !RowName.IsNone())
            {
                const uint8* RawRow = DataTable->FindRowUnchecked(RowName);
                const UScriptStruct* RowStruct = DataTable->GetRowStruct();
                if (RawRow && RowStruct)
                {
                    for (TFieldIterator<const FProperty> It(RowStruct); It; ++It)
                    {
                        const FProperty* Prop = *It;
                        FString DisplayName = Prop->GetName();
                        /*FString Value;
                        Prop->ExportText_Direct(Value, Prop->ContainerPtrToValuePtr<void>(RawRow), nullptr, nullptr, 0);
                        float FloatValue = FCString::Atof(*Value);*/
                        if (DisplayName.Contains(TEXT("Cost")))
                        {
                            FString ValueStr;
                            Prop->ExportText_Direct(ValueStr, Prop->ContainerPtrToValuePtr<void>(RawRow), nullptr, nullptr, 0);
                            //const void* ValuePtr = Prop->ContainerPtrToValuePtr<void>(RawRow);
                            TotalAbilityCost = FCString::Atof(*ValueStr);
                        }
                    }
                }
            }
        }
    }
}
#endif

bool UChitaGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
    {
        return false;
    }

    UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
    float RemainingCost = TotalAbilityCost;

    const int32 LastIndex = CostGameplayEffectClasses.Num() - 1;

    for (int32 i = 0; i < CostGameplayEffectClasses.Num(); ++i)
    {
        const TSubclassOf<UGameplayEffect>& CostGEClass = CostGameplayEffectClasses[i];
        if (!CostGEClass || RemainingCost <= 0.f)
        {
            continue;
        }

        const UGameplayEffect* CostGECDO = CostGEClass->GetDefaultObject<UGameplayEffect>();
        if (!CostGECDO || CostGECDO->Modifiers.Num() == 0)
        {
            continue;
        }

        const FGameplayModifierInfo& Modifier = CostGECDO->Modifiers[0];
        if (!Modifier.Attribute.IsValid())
        {
            continue;
        }

        float CurrentValue = ASC->GetNumericAttribute(Modifier.Attribute);
        if (CurrentValue <= 0.f)
        {
            continue;
        }

        // If this is the last GE, make sure it won't reduce the value to zero or below
        if (i == LastIndex)
        {
            float Needed = RemainingCost;
            if (CurrentValue - Needed <= 0.f)
            {
                if (OptionalRelevantTags)
                {
                    OptionalRelevantTags->AddTag(UAbilitySystemGlobals::Get().ActivateFailCostTag);
                }

                UE_LOG(LogTemp, Warning, TEXT("CheckCost: Last fallback resource (e.g. HP) would be depleted. Failing cost check."));
                return false;
            }
        }

        float CostToCover = FMath::Min(CurrentValue, RemainingCost);
        RemainingCost -= CostToCover;
    }

    if (RemainingCost > 0.f)
    {
        if (OptionalRelevantTags)
        {
            OptionalRelevantTags->AddTag(UAbilitySystemGlobals::Get().ActivateFailCostTag);
        }

        UE_LOG(LogTemp, Warning, TEXT("CheckCost: Not enough resources to cover ability cost."));
        return false;
    }

    return true;
}



void UChitaGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo) const
{
    if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
    {
        return;
    }

    UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
    const int32 AbilityLevel = GetAbilityLevel(Handle, ActorInfo);

    float RemainingCost = TotalAbilityCost;

    for (const TSubclassOf<UGameplayEffect>& CostGEClass : CostGameplayEffectClasses)
    {
        if (!CostGEClass || RemainingCost <= 0.f)
        {
            continue;
        }

        const UGameplayEffect* CostGECDO = CostGEClass->GetDefaultObject<UGameplayEffect>();
        if (!CostGECDO || CostGECDO->Modifiers.Num() == 0)
        {
            continue;
        }

        // Get the attribute this GE modifies
        const FGameplayModifierInfo& Modifier = CostGECDO->Modifiers[0];
        if (!Modifier.Attribute.IsValid())
        {
            continue;
        }

        // Get current value of the target attribute
        float CurrentValue = ASC->GetNumericAttribute(Modifier.Attribute);
        if (CurrentValue <= 0.f)
        {
            continue;
        }

        float CostToApply = FMath::Min(CurrentValue, RemainingCost);

        // Create effect spec
        FGameplayEffectContextHandle Context = MakeEffectContext(Handle, ActorInfo);
        FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CostGEClass, AbilityLevel, Context);

        if (SpecHandle.IsValid())
        {
            // Infer SetByCaller tag from attribute name: Data.<AttrName>Cost
            FString AttributeName = Modifier.Attribute.GetName(); // e.g., "Focus"
            UE_LOG(LogTemp, Warning, TEXT("%s"), *AttributeName);
            FString TagName = FString::Printf(TEXT("Data.%sCost"), *AttributeName);
            FGameplayTag CostTag = FGameplayTag::RequestGameplayTag(*TagName);

            SpecHandle.Data->SetSetByCallerMagnitude(CostTag, -CostToApply);
            ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

            RemainingCost -= CostToApply;
        }
    }

    if (RemainingCost > 0.f)
    {
        UE_LOG(LogTemp, Warning, TEXT("ApplyCost(): Not enough resources. %f cost couldn't be paid!"), RemainingCost);
    }
}



