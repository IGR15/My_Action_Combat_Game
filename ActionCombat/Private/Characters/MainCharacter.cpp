// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"
#include "Animations/PlayerAnimInstance.h"
#include "Characters/StatsComponent.h"
#include "Characters/EStat.h"
#include "Combat/CombatComponent.h"
#include "Combat/BlockComponent.h"
#include "Combat/TraceComponent.h"
#include "Characters/TeleportComponent.h"
#include "Combat/LockonComponent.h"
#include "Characters/PlayerActionsComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats Component"));
	LockonComp = CreateDefaultSubobject<ULockonComponent>(TEXT("Lockon Component"));
	BlockComp = CreateDefaultSubobject<UBlockComponent>(TEXT("Block Component"));
	TraceComp = CreateDefaultSubobject<UTraceComponent>(TEXT("Trace Component"));
	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
	PlayerActionComp = CreateDefaultSubobject<UPlayerActionsComponent>(TEXT("Player Action Component"));
	TeleportComp = CreateDefaultSubobject<UTeleportComponent>(TEXT("Teleport Component"));
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AMainCharacter::GetDamage()
{
	return StatsComp->Stats[EStat::Strength];
}

bool AMainCharacter::HasEnoughStamina(float Cost)
{
	return StatsComp->Stats[EStat::Stamina] >= Cost;
}

void AMainCharacter::HandleDeath()
{
	PlayAnimMontage(DeathAnimMontage);
	 
	DisableInput(GetController<APlayerController>());
}

void AMainCharacter::EndLockonWithActor(AActor* Actorref)
{
	if(LockonComp-> CurrentTargetActor !=Actorref){
		return;
	}
	LockonComp->EndLockon();
}

bool AMainCharacter::CanTakeDamage(AActor* Opponent)
{
	if (PlayerActionComp->bIsRollActive) { return false; }

	if (PlayerAnim->bIsBlocking)
	{
		return BlockComp->Check(Opponent);
		
	}

	return true;
}

void AMainCharacter::PlayHurtAnim(TSubclassOf<class UCameraShakeBase> CameraShakeTemplate)
{
	PlayAnimMontage(HurtAnimMontage);

	if(CameraShakeTemplate)
	{
		GetController<APlayerController>()->ClientStartCameraShake(CameraShakeTemplate);
	}
}

