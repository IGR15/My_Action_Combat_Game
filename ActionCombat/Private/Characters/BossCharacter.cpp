// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BossCharacter.h"
#include "Characters/StatsComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Combat/CombatComponent.h"
#include "characters/MainCharacter.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/MainPlayer.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats Component"));
	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));

}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();

	ControllerRef = GetController<AAIController>();
	BlackBoardComp = ControllerRef->GetBlackboardComponent();

	BlackBoardComp->SetValueAsEnum(
		TEXT("CurrentState"),
		InitialState
	);

	GetWorld()->GetFirstPlayerController()->GetPawn<AMainCharacter>()
		->StatsComp
		->OnZeroHealthDelegate.AddDynamic(this, &ABossCharacter::HandlePlayerDeath);
	//this is to subscribe to the player's death event 
	
}

void ABossCharacter::DetectPawn(APawn* DetectedPawn, APawn* PawnToDetect)
{
	EEnemyState CurrentState{ static_cast<EEnemyState>(BlackBoardComp->GetValueAsEnum(TEXT("CurrentState"))) };
	if (DetectedPawn != PawnToDetect || CurrentState != EEnemyState::Idle) { return; }
	BlackBoardComp->SetValueAsEnum(
		TEXT("CurrentState"),
		EEnemyState::Range
	);
}

float ABossCharacter::GetDamage()
{
	return StatsComp->Stats[EStat::Strength];
}

void ABossCharacter::Attack()
{
	CombatComp->RandomAttack();
}

float ABossCharacter::getAnimDuration()
{
	return CombatComp->AnimDuration;
}

float ABossCharacter::GetMaleeRange()
{
	return StatsComp->Stats[EStat::MaleeRange];
}

void ABossCharacter::HandlePlayerDeath()
{
	ControllerRef->GetBlackboardComponent()->SetValueAsEnum(
		TEXT("CurrentState"),
		EEnemyState::GameOver
	);
}

void ABossCharacter::HandleDeath()
{
	float Duration{ PlayAnimMontage(DeathAnim) };

	ControllerRef->GetBrainComponent()->StopLogic("Defeated");

	FindComponentByClass<UCapsuleComponent>()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle DestroyTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		DestroyTimerHandle,
		this,
		&ABossCharacter::FinsihDeathAnim,
		Duration,
		false
	);

	IMainPlayer* PlayerInterface(GetWorld()->GetFirstPlayerController()->GetPawn<AMainCharacter>());
	if (!PlayerInterface)
	{
		return;
	}
	PlayerInterface->EndLockonWithActor(this);
	
}

void ABossCharacter::FinsihDeathAnim()
{
	Destroy();
}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

