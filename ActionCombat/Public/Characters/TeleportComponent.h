// TeleportComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "Curves/CurveFloat.h"
#include "TeleportComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ACTIONCOMBAT_API UTeleportComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTeleportComponent();

	void StartTeleportAim();
	void UpdateTeleportAim(float DeltaTime);
	void CompleteTeleport();
	void CancelTeleport();

protected:
	virtual void BeginPlay() override;

private:
	// Configurable properties
	UPROPERTY(EditAnywhere)
	USceneComponent* TeleportAura;

	UPROPERTY(EditAnywhere)
	USceneComponent* TraceStart;

	UPROPERTY(EditAnywhere)
	float MaxTraceDistance = 1000.0f;

	UPROPERTY(EditAnywhere)
	float DownTraceDepth = 500.0f;

	UPROPERTY(EditAnywhere)
	UCurveFloat* TeleportCurve;

	// Internal state
	FVector TeleportTargetLocation;
	FVector StartLocation;
	bool bIsAiming = false;
	bool bCanTeleport = false;
	

	// Cached Owner Character
	ACharacter* OwnerCharacter;

	// Helpers
	void PerformAimTrace();
	void PerformGroundCheck(const FVector& AimPoint);
	void UpdateAura(const FVector& Location, bool bVisible);
	void ExecuteTeleport();
	void TimelineUpdate(float Value);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
