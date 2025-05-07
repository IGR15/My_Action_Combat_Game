// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/LockonComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/Enemy.h"


// Sets default values for this component's properties
ULockonComponent::ULockonComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULockonComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerRef = GetOwner<ACharacter>();
	Controller = GetWorld()->GetFirstPlayerController();
	Movementcomp = OwnerRef->GetCharacterMovement();
	SpringArmComp = OwnerRef->FindComponentByClass<USpringArmComponent>();
	
}

void ULockonComponent::StartLockon(float SphereRadius)
{
	FHitResult OutResult;
	FVector currentLocation{ OwnerRef->GetActorLocation() };
	FCollisionShape Sphere{ FCollisionShape::MakeSphere(SphereRadius) };
	FCollisionQueryParams IgnoreParams{
		FName(TEXT("Ignor Wuery Paramaters")),
		false,
		OwnerRef
	};
	bool hasFoundTarget{ GetWorld()->SweepSingleByChannel(
		OutResult,
		currentLocation,
		currentLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		Sphere,
		IgnoreParams



	) };
	if (!hasFoundTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("No target found"));
		return;
	}

	if (!OutResult.GetActor()->Implements<UEnemy>()) { return; }
		
	
	CurrentTargetActor = OutResult.GetActor();
	Controller->SetIgnoreLookInput(true);
	Movementcomp->bOrientRotationToMovement = false;
	Movementcomp->bUseControllerDesiredRotation = true;
	SpringArmComp->TargetOffset = FVector{0.0f, 0.0f, 100.0f};
	IEnemy::Execute_OnSelect(CurrentTargetActor);

	OnUpdatedTargetDelegate.Broadcast(CurrentTargetActor);
	
}

void ULockonComponent::EndLockon()
{
	if (IsValid(CurrentTargetActor))
	{
		IEnemy::Execute_OnDeselect(CurrentTargetActor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No target to deselect"));
	}
	CurrentTargetActor = nullptr;
	Controller->ResetIgnoreLookInput();
	Movementcomp->bOrientRotationToMovement = true;
	Movementcomp->bUseControllerDesiredRotation = false;
	SpringArmComp->TargetOffset = FVector::ZeroVector;
	UE_LOG(LogTemp, Warning, TEXT("End Lockon"));
	OnUpdatedTargetDelegate.Broadcast(CurrentTargetActor);
	
	
}

void ULockonComponent::ToggleLockon(float Radius)
{
	if (IsValid(CurrentTargetActor) )
	{
		EndLockon();
	}
	else
	{
		StartLockon(Radius);
	}
}


// Called every frame
void ULockonComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(CurrentTargetActor)) { return; }

	FVector CurrentLocation{ OwnerRef->GetActorLocation() };
	FVector TargetLocation{ CurrentTargetActor->GetActorLocation() };
	double TargetDistance{
		FVector::Distance(CurrentLocation, TargetLocation)
	};
	if (TargetDistance >= BreakDistance)
	{
		EndLockon();
		return;
	}
	TargetLocation.Z -= 125;
	FRotator NewRotator{ UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation) };

	Controller->SetControlRotation(NewRotator);


}

	

