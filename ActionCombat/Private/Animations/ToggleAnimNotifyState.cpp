// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/ToggleAnimNotifyState.h"
#include "Combat/TraceComponent.h"

void UToggleAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	UTraceComponent* TraceComp{ MeshComp->GetOwner()->FindComponentByClass<UTraceComponent>() };
	if (!IsValid( TraceComp))
	{
		return;
	}
	TraceComp->bIsAttacking = true;
}

void UToggleAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	UTraceComponent* TraceComp{ MeshComp->GetOwner()->FindComponentByClass<UTraceComponent>() };
	if (!IsValid(TraceComp))
	{
		return;
	}
	TraceComp->bIsAttacking = false;
}
