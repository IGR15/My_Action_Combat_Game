// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MyCharacterAttributeSet.generated.h"

/**
 * 
 */
#define ATTRIBUTE_ACCESSORS_BASIC(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttributeChangedEvent, UAttributeSet*, AttributeSet, float, OldValue, float, NewValue);

UCLASS()
class ACTIONCOMBAT_API UMyCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()


	


public:
	UMyCharacterAttributeSet();
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Health;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Stamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Mana;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxMana;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;


	ATTRIBUTE_ACCESSORS_BASIC(UMyCharacterAttributeSet, MaxMana)
	ATTRIBUTE_ACCESSORS_BASIC(UMyCharacterAttributeSet, Mana)
	ATTRIBUTE_ACCESSORS_BASIC(UMyCharacterAttributeSet, MaxStamina)
	ATTRIBUTE_ACCESSORS_BASIC(UMyCharacterAttributeSet, Stamina)
	ATTRIBUTE_ACCESSORS_BASIC(UMyCharacterAttributeSet, MaxHealth)
	ATTRIBUTE_ACCESSORS_BASIC(UMyCharacterAttributeSet, Health)

	UPROPERTY(BlueprintAssignable)
	FAttributeChangedEvent OnHealthChanged;
	
};
