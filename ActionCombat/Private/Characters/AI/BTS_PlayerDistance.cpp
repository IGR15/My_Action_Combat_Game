// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTS_PlayerDistance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTS_PlayerDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FVector CurrentLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();

	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	float Distance = static_cast<float>(FVector::Distance(CurrentLocation, PlayerLocation));//Blackboard dose not support double type thats why we use float

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("Distance"), Distance);
}
