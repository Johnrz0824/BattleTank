// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "BattleTank.h"

#define OUT
void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (ensure(AimingComponent))
		FoundAimingComponent(AimingComponent);
	else
		UE_LOG(LogTemp, Warning, TEXT("No Aiming Component"));
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
	
}

void ATankPlayerController::AimTowardsCrosshair()
{
	auto Pawn=GetPawn();
	if (!Pawn) { return; }
	auto AimingComponent = Pawn->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }
	FVector HitLocation;
	AimingComponent->UpdateState();
	bool bGotHitLocation = GetSightRayHitLocation(OUT HitLocation);
	if (bGotHitLocation)
	{
		AimingComponent->AimAt(HitLocation);
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector &hitPos) const
{
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return false; }
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(OUT ViewportSizeX, OUT ViewportSizeY);
	FVector2D ScreenLocation(ViewportSizeX * CrosshairXLocation, ViewportSizeY* CrosshairYLocation);
	FVector WorldDirection;
	if(GetLookDirection(ScreenLocation, OUT WorldDirection))
	{
		return GetLookVectorHitLocation(WorldDirection,OUT hitPos);
	}
	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection)const
{
	FVector CameraWorldLocation;
	return (DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, OUT LookDirection));
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation)const
{
	FHitResult hit;
	float reach = 1000 * 100;
	FVector ShootStart = PlayerCameraManager->GetCameraLocation();
	FVector LineTraceEnd = ShootStart + LookDirection * reach;
	bool bGet = GetWorld()->LineTraceSingleByChannel(
		OUT hit,
		ShootStart,
		LineTraceEnd,
		ECollisionChannel::ECC_Visibility
		);
	HitLocation = hit.Location;
	return bGet;
}


