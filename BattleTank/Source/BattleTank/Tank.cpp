// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankBarrel.h"
#include "Projectile.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"


ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATank::AimAt(FVector HitLocation)
{
	if(TankAimingComponent)
		TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}

void ATank::Fire()
{
	auto requestTime = GetWorld()->GetTimeSeconds();
	bool isReloaded = (requestTime - LastFireTime) > ReloadTime;
	if (!isReloaded || !Barrel) { return; }
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint
		,Barrel->GetSocketLocation(FName("Projectile"))
		,Barrel->GetSocketRotation(FName("Projectile"))
		,FActorSpawnParameters()
		);
	Projectile->LaunchProjectile(LaunchSpeed);
	LastFireTime = requestTime;

}