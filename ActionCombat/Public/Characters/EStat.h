// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum EStat
{
	none UMETA(DisplayName = "None Selected"),
	Health UMETA(DisplayName = "Health"),
	Stamina UMETA(DisplayName = "Stamina"),
	MaxHealth UMETA(DisplayName = "Max Health"),
	MaxStamina UMETA(DisplayName = "Max Stamina"),
	Strength UMETA(DisplayName = "Strength"),

};