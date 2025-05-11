// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCOMBAT_API UBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentSpeed{ 0.0f };
	/*UFUNCTION(BlueprintCallable, Category = "Animation")
	void UpdateSpeed();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInCombat{ false };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentDirection{ 0.0f };*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCharging{ false };
	
};
