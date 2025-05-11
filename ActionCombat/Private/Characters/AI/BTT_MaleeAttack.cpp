// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTT_MaleeAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"//for the moveRequest

EBTNodeResult::Type UBTT_MaleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bIsFinished = false;

	float Distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Distance"));

	if (Distance > AttackRadius)
	{
		APawn* PlayerRef{ GetWorld()->GetFirstPlayerController()->GetPawn() };

		FAIMoveRequest MoveRequest{ PlayerRef };
		MoveRequest.SetUsePathfinding(true);
		MoveRequest.SetAcceptanceRadius(AcceptableRadius);

		OwnerComp.GetAIOwner()->ReceiveMoveCompleted.AddUnique(MoveDelegate);//we put the binding above the move request to avoid the crash

		OwnerComp.GetAIOwner()->MoveTo(MoveRequest);
		OwnerComp.GetAIOwner()->SetFocus(PlayerRef);

		
		
	}

	return EBTNodeResult::InProgress;
}

void UBTT_MaleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!bIsFinished)
	{
		return;
	}
	OwnerComp.GetAIOwner()->ReceiveMoveCompleted.Remove(MoveDelegate);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

UBTT_MaleeAttack::UBTT_MaleeAttack()
{
	MoveDelegate.BindUFunction(
		this,
		"FinishAttackTask"
	);
	bNotifyTick = true;
}

void UBTT_MaleeAttack::FinishAttackTask()
{
	bIsFinished = true;
	/*AAIController* ControllerRef{ Cast<AAIController>(GetWorld()->GetFirstPlayerController()) };
	if (ControllerRef)
	{
		ControllerRef->ReceiveMoveCompleted.Remove(MoveDelegate);
	}*/
}
