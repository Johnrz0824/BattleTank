// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "BattleTank.h"
#include "Tank.h"
#include "Mortar.h"
void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PossededTank = Cast<ATank>(InPawn);
		if (ensure(PossededTank)) { PossededTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnTankDeath); }
		auto PossededMortar = Cast<AMortar>(InPawn);
		if (ensure(PossededMortar)) { PossededMortar->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnTankDeath); }
	}
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::OnTankDeath()
{
	GetPawn()->DetachFromControllerPendingDestroy();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto ControlledTank = GetPawn();

	if (!ensure(Player && ControlledTank)) { return; }
	MoveToActor(Player, AcceptanceRadius);

	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	AimingComponent->AimAt(Player->GetActorLocation());
	AimingComponent->UpdateState();
	if(AimingComponent->GetFiringState() == EFiringStatus::Locked)
		AimingComponent->Fire();
}