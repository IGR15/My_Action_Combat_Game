// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTT_MaleeAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"//for the moveRequest
#include "Interfaces/Fighter.h"
#include "GameFramework/Character.h"
#include "Characters/EEnemyState.h"

EBTNodeResult::Type UBTT_MaleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	

	bIsFinished = false;

	

	float Distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Distance"));
	AAIController* AIControllerRef{ OwnerComp.GetAIOwner() };

	

	if (Distance > AttackRadius)
	{
		APawn* PlayerRef{ GetWorld()->GetFirstPlayerController()->GetPawn() };

		FAIMoveRequest MoveRequest{ PlayerRef };
		MoveRequest.SetUsePathfinding(true);
		MoveRequest.SetAcceptanceRadius(AcceptableRadius);

		AIControllerRef->ReceiveMoveCompleted.AddUnique(MoveDelegate);//we put the binding above the move request to avoid the crash

		AIControllerRef->MoveTo(MoveRequest);
		AIControllerRef->SetFocus(PlayerRef);

		
		
	}
	else
	{
		IFighter* FighterRef{ Cast<IFighter>(AIControllerRef->GetCharacter()) };

		FighterRef->Attack();
		 
		FTimerHandle TimerHandle;
		AIControllerRef->GetCharacter()->GetWorldTimerManager().SetTimer(
			TimerHandle,
			this,
			&UBTT_MaleeAttack::FinishAttackTask,
			FighterRef->getAnimDuration(),
			false
			
		);
	}

	return EBTNodeResult::InProgress;
}

void UBTT_MaleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	float Distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Distance"));

	AAIController* AIControllerRef{ OwnerComp.GetAIOwner()};
	
	IFighter* FighterRef{ Cast<IFighter>(AIControllerRef->GetCharacter()) };


	if (Distance > FighterRef->GetMaleeRange()) {

		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(
			TEXT("CurrentState"),
			EEnemyState::Range
		);
		AbortTask(OwnerComp,NodeMemory);
		FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);

		AIControllerRef->StopMovement();
		AIControllerRef->ClearFocus(EAIFocusPriority::Gameplay);
		AIControllerRef->ReceiveMoveCompleted.Remove(MoveDelegate);
	}

	if (!bIsFinished)
	{
		return;
	}
	OwnerComp.GetAIOwner()->ReceiveMoveCompleted.Remove(MoveDelegate);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

UBTT_MaleeAttack::UBTT_MaleeAttack()
{
	bCreateNodeInstance = true;
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
