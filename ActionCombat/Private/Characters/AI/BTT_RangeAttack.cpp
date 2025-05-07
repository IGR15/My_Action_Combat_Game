// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTT_RangeAttack.h"
#include "AIController.h"
#include "GameFramework/character.h"

EBTNodeResult::Type UBTT_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* CharacterRef{ OwnerComp.GetAIOwner()->GetPawn<ACharacter>()};
	if (!IsValid(CharacterRef))
	{
		// Task fails if the character is invalid
		return EBTNodeResult::Failed;
	};
	CharacterRef->PlayAnimMontage(AnimkMontage);
	return EBTNodeResult::Succeeded;
}
