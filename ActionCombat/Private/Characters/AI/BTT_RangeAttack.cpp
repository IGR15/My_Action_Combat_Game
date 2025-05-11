// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTT_RangeAttack.h"
#include "AIController.h"
#include "GameFramework/character.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EEnemyState.h"

EBTNodeResult::Type UBTT_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* CharacterRef{ OwnerComp.GetAIOwner()->GetPawn<ACharacter>()};
	if (!IsValid(CharacterRef))
	{
		// Task fails if the character is invalid
		return EBTNodeResult::Failed;
	};
	float Distance{ OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Distance")) };
	if(Distance<MeleeRange){
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(
			TEXT("CurrentState"),
			EEnemyState::Melee
		);

		AbortTask(OwnerComp, NodeMemory);//we abourt the tack because we are in melee range,this clear the memory but dose not abourt the function

		return EBTNodeResult::Failed;// this aborts the function
	
	}
	CharacterRef->PlayAnimMontage(AnimkMontage);
	double RandomValue{UKismetMathLibrary::RandomFloat()};
	if (RandomValue > Threshold)
	{
		Threshold = 0.9;
		OwnerComp.GetBlackboardComponent()
			->SetValueAsEnum(
				TEXT("CurrentState"),
				EEnemyState::Charge
			);
	}
	else
	{
		Threshold -= 0.1;
		
	}
	
	return EBTNodeResult::Succeeded;
}
