// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "BattleTank.h"


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto ControlledTank = GetPawn();

	if (!ensure(Player && ControlledTank)) { return; }
	MoveToActor(Player, AcceptanceRadius);

	UE_LOG(LogTemp, Warning, TEXT("3"));
	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	AimingComponent->AimAt(Player->GetActorLocation());
	AimingComponent->UpdateState();
	UE_LOG(LogTemp, Warning, TEXT("4"));
	if(AimingComponent->GetFiringState() == EFiringStatus::Locked)
		AimingComponent->Fire();
}