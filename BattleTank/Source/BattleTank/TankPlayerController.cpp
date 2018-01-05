// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"
#include "Tank.h"
#include "BattleTank.h"

#define OUT
void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) { return; }

	FVector HitLocation;
	if (GetSightRayHitLocation(OUT HitLocation))
	{

	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector &hitPos) const
{
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(OUT ViewportSizeX, OUT ViewportSizeY);
	FVector2D ScreenLocation(ViewportSizeX * CrosshairXLocation, ViewportSizeY* CrosshairYLocation);
	FVector WorldDirection;
	if(GetLookDirection(ScreenLocation, OUT WorldDirection))
	{
		FHitResult hit;
		if (GetLookVectorHitLocation(OUT hit, WorldDirection))
		{
			GetControlledTank()->AimAt(hit.Location);
		}
	}
	return true;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection)const
{
	FVector CameraWorldLocation;
	return (DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, OUT LookDirection));
}

bool ATankPlayerController::GetLookVectorHitLocation(FHitResult& hit, FVector LookDirection)const
{
	float reach = 1000 * 100;
	FVector ShootStart = PlayerCameraManager->GetCameraLocation();
	FVector LineTraceEnd = ShootStart + LookDirection * reach;
	return GetWorld()->LineTraceSingleByChannel(
		OUT hit,
		ShootStart,
		LineTraceEnd,
		ECollisionChannel::ECC_Visibility
		);
}


