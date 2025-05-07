// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/LookAtPlayerAnimNotifyState.h"
#include "Characters/LookAtPlayerComponent.h"

void ULookAtPlayerAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AActor* OwnerRef{ MeshComp->GetOwner() };
	if (!IsValid(OwnerRef)) { return; }
	ULookAtPlayerComponent* LookAtPlayerComp{ OwnerRef->FindComponentByClass<ULookAtPlayerComponent>() };
	if (!IsValid(LookAtPlayerComp)) { return; }
	LookAtPlayerComp->bCanRotate = true;
}

void ULookAtPlayerAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* OwnerRef{ MeshComp->GetOwner() };
	if (!IsValid(OwnerRef)) { return; }
	ULookAtPlayerComponent* LookAtPlayerComp{ OwnerRef->FindComponentByClass<ULookAtPlayerComponent>() };
	if (!IsValid(LookAtPlayerComp)) { return; }
	LookAtPlayerComp->bCanRotate = false;
}
