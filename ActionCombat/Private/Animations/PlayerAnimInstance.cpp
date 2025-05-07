// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/PlayerAnimInstance.h"

void UPlayerAnimInstance::UpdateSpeed()
{// Get the owning actor
	if (APawn* Character = Cast<APawn>(TryGetPawnOwner()))
	{
		// Get the velocity of the character
		FVector Velocity = Character->GetVelocity();

		// Calculate the speed
		CurrentSpeed = static_cast<float> (Velocity.Length());
	}
}

void UPlayerAnimInstance::HandleUpdatedTarget(AActor* NewTargetActorRef)
{
	bIsInCombat = IsValid(NewTargetActorRef);
	
}

void UPlayerAnimInstance::UpdateDirection()
{
	APawn* PawnRef{ TryGetPawnOwner()};
	if (!IsValid(PawnRef)) { return; }

	if (!bIsInCombat) { return; }


	CurrentDirection = CalculateDirection(
		PawnRef->GetVelocity(),
		PawnRef->GetActorRotation()
	
	);
}
