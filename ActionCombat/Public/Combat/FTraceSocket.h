// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FTraceSocket.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct ACTIONCOMBAT_API FTraceSocket
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName Start;


	UPROPERTY(EditAnywhere)
	FName End;

	UPROPERTY(EditAnywhere)
	FName Rotation;
};
