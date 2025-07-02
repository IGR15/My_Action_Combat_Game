// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/TeleportComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

UTeleportComponent::UTeleportComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTeleportComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (TeleportAura)
	{
		TeleportAura->SetVisibility(false);
	}

}

void UTeleportComponent::StartTeleportAim()
{
	bIsAiming = true;
	if (TeleportAura)
	{
		TeleportAura->SetVisibility(true);
	}
}

void UTeleportComponent::UpdateTeleportAim(float DeltaTime)
{
	if (!bIsAiming || !OwnerCharacter) return;
	PerformAimTrace();
}

void UTeleportComponent::CancelTeleport()
{
	bIsAiming = false;
	bCanTeleport = false;
	if (TeleportAura)
	{
		TeleportAura->SetVisibility(false);
	}
}

void UTeleportComponent::CompleteTeleport()
{
	if (!bIsAiming || !bCanTeleport || !OwnerCharacter) return;
	bIsAiming = false;
	TeleportAura->SetVisibility(false);
	StartLocation = OwnerCharacter->GetActorLocation();
}

void UTeleportComponent::PerformAimTrace()
{
	FVector Start = TraceStart ? TraceStart->GetComponentLocation() : OwnerCharacter->GetActorLocation();
	FVector End = Start + OwnerCharacter->GetActorForwardVector() * MaxTraceDistance;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	if (bHit)
	{
		PerformGroundCheck(Hit.ImpactPoint);
	}
	else
	{
		UpdateAura(FVector::ZeroVector, false);
		bCanTeleport = false;
	}
}

void UTeleportComponent::PerformGroundCheck(const FVector& AimPoint)
{
	FVector GroundStart = AimPoint + FVector(0, 0, 50);
	FVector GroundEnd = AimPoint - FVector(0, 0, DownTraceDepth);

	FHitResult GroundHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

	bool bGround = GetWorld()->LineTraceSingleByChannel(GroundHit, GroundStart, GroundEnd, ECC_Visibility, Params);
	if (bGround)
	{
		TeleportTargetLocation = GroundHit.ImpactPoint;
		UpdateAura(TeleportTargetLocation, true);
		bCanTeleport = true;
	}
	else
	{
		UpdateAura(FVector::ZeroVector, false);
		bCanTeleport = false;
	}
}

void UTeleportComponent::UpdateAura(const FVector& Location, bool bVisible)
{
	if (!TeleportAura) return;
	TeleportAura->SetWorldLocation(Location);
	TeleportAura->SetVisibility(bVisible);
}

void UTeleportComponent::ExecuteTeleport()
{
	if (OwnerCharacter)
	{
		OwnerCharacter->SetActorLocation(TeleportTargetLocation);
	}
}

void UTeleportComponent::TimelineUpdate(float Value)
{
	if (!OwnerCharacter) return;
	FVector NewLocation = FMath::Lerp(StartLocation, TeleportTargetLocation, Value);
	OwnerCharacter->SetActorLocation(NewLocation);
}



// Called every frame
void UTeleportComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

