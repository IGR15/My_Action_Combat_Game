// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/MainPlayer.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	CharacterRef = GetOwner<ACharacter>();
	
}

void UCombatComponent::ComboAttack()
{
	if(CharacterRef->Implements<UMainPlayer>())
	{
		IMainPlayer* IPlayerRef{ Cast<IMainPlayer>(CharacterRef) };
		if (IPlayerRef && !IPlayerRef->HasEnoughStamina(StamaninaCost))
		{
			return;
		}
	}
	if (!bCanAttack)
	{
		return;
	}
	bCanAttack = false;
	CharacterRef->PlayAnimMontage(AttackAnimations[combosCount]);
	combosCount++;
	int maxCombos{ AttackAnimations.Num() };
	combosCount = UKismetMathLibrary::Wrap(
	combosCount,
		-1,
		maxCombos - 1
		
	);
	OnAttackPerformedDelegate.Broadcast(StamaninaCost); // Notify that an attack has been performed
}

void UCombatComponent::HandleResetAttack()
{
	bCanAttack = true;
}

void UCombatComponent::RandomAttack()
{
	int RandomIndex{ FMath::RandRange(0, AttackAnimations.Num() - 1) };

	AnimDuration = CharacterRef->PlayAnimMontage(AttackAnimations[RandomIndex]);

}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

