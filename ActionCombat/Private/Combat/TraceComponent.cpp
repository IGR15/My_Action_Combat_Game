// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/TraceComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DamageEvents.h"
#include "Interfaces/Fighter.h"
#include "Kismet/GameplayStatics.h"



// Sets default values for this component's properties
UTraceComponent::UTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	SkeletalComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	
}


// Called every frame
void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsAttacking) { return; }

	TArray <FHitResult>AllResults;

	for(const FTraceSocket Scoket: Sockets){
		FVector StartSocketLocation{ SkeletalComp->GetSocketLocation(Scoket.Start) };
		FVector EndSocketLocation{ SkeletalComp->GetSocketLocation(Scoket.End) };
		FQuat ShapeRotation{ SkeletalComp->GetSocketQuaternion(Scoket.Rotation) };
		TArray <FHitResult>OutResults;
		double WeaponDistance{ FVector::Distance(StartSocketLocation, EndSocketLocation) };

		FVector BoxHalfExtent{ BoxCollisionLength,BoxCollisionLength,WeaponDistance };
		BoxHalfExtent /= 2;
		FCollisionShape BoxShape{ FCollisionShape::MakeBox(BoxHalfExtent) };
		FCollisionQueryParams IgnoreParams{
			FName(TEXT("Ignore Paramas")),
			false,
			GetOwner()
		};
		bool HasFoundTarget{ GetWorld()->SweepMultiByChannel(
			OutResults,
			StartSocketLocation,
			EndSocketLocation,
			ShapeRotation,
			ECollisionChannel::ECC_GameTraceChannel1,
			BoxShape,
			IgnoreParams
		) };
		for (FHitResult Hit: OutResults) {
			AllResults.Add(Hit);
		}
		if (bDebugMode) {
			FVector CenterPoint{
				UKismetMathLibrary::VLerp(
					StartSocketLocation,
					EndSocketLocation,
					0.5f
				)
			};
			UKismetSystemLibrary::DrawDebugBox(
				GetWorld(),
				CenterPoint,
				BoxShape.GetExtent(),
				HasFoundTarget ? FLinearColor::Green : FLinearColor::Red,
				ShapeRotation.Rotator(),
				1.0f,
				2.0f

			);

		}
	
	}

	if (AllResults.Num()==0) { return; }


	float CharacterDamage{ 0.0f };

	IFighter* FighterRef{ Cast<IFighter>(GetOwner()) };
	if (FighterRef)
	{
		CharacterDamage = FighterRef->GetDamage();
	}
	FDamageEvent TargetAttackedEvent;
	for (const FHitResult& Hit : AllResults)
	{
		AActor* TargetActor{ Hit.GetActor() };
		if(TargetsToIgnor.Contains(TargetActor))
		{
			continue;
		}
		TargetActor->TakeDamage(
			CharacterDamage,
			TargetAttackedEvent,
			GetOwner()->GetInstigatorController(),
			GetOwner()

		);
	
		TargetsToIgnor.AddUnique(TargetActor);

		UGameplayStatics::SpawnEmitterAtLocation
		(
			GetWorld(),
			HitParticleTemplate,
			Hit.ImpactPoint
		);

	}
}

void UTraceComponent::HandleResetAttack()
{
	TargetsToIgnor.Empty();
	
}

	

