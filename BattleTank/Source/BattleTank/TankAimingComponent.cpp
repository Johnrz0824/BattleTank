// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;

	bAutoRegister = true;
	bWantsInitializeComponent = true;
	bAutoActivate = true;
}

void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	PrimaryComponentTick.bCanEverTick = true;
	LastFireTime = FPlatformTime::Seconds();
	Ammos = 5;
}

void UTankAimingComponent::UpdateState()
{
	if (Ammos <= 0)
	{
		FiringState = EFiringStatus::OutOfAmmo;
		return;
	}

	if (FPlatformTime::Seconds() - LastFireTime < ReloadTime)
		FiringState = EFiringStatus::Reloading;
	else if (IsBarrelMoving())
		FiringState = EFiringStatus::Aiming;
	else
		FiringState = EFiringStatus::Locked;
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }
	auto BarrelForward = Barrel->GetForwardVector();
	return !BarrelForward.Equals(AimDirection,0.1);
}

void UTankAimingComponent::AimAt(FVector WorldSpaceAim)
{
	if (!ensure(Barrel)) { return; }
	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	bool success = UGameplayStatics::SuggestProjectileVelocity(
		Barrel,
		OutLaunchVelocity,
		StartLocation,
		WorldSpaceAim,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);
	if (!success) { return; }
	AimDirection = OutLaunchVelocity.GetSafeNormal();
	MoveBarrelTowards(AimDirection);
}


void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	Barrel = BarrelToSet;
}

void UTankAimingComponent::SetTurretReference(UTankTurret* TurretToSet)
{
	Turret = TurretToSet;
}

void UTankAimingComponent::MoveBarrelTowards(FVector Direction)
{
	auto BarrelRotation = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = Direction.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotation;
	Barrel->Elevate(DeltaRotator.Pitch);
	if(FMath::Abs(DeltaRotator.Yaw) > 180)
		Turret->Elevate(-DeltaRotator.Yaw);
	else
		Turret->Elevate(DeltaRotator.Yaw);
}

void UTankAimingComponent::Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	if (!ensure(BarrelToSet && TurretToSet)) { return; }
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}



void UTankAimingComponent::Fire()
{
	if (FiringState == EFiringStatus::Reloading || FiringState == EFiringStatus::OutOfAmmo) { return; }
	if (!ensure(Barrel && ProjectileBlueprint)) { return; }
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint
		, Barrel->GetSocketLocation(FName("Projectile"))
		, Barrel->GetSocketRotation(FName("Projectile"))
		, FActorSpawnParameters()
		);
	Projectile->LaunchProjectile(4000);
	LastFireTime = FPlatformTime::Seconds();
	Ammos--;

}

EFiringStatus UTankAimingComponent::GetFiringState()const
{
	return FiringState;
}