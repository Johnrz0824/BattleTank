// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankPlayerController.h"
#include "Engine/World.h"
#include "Tank.h"
#include "BattleTank.h"


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto Player = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	auto AI = Cast<ATank>(GetPawn());
	if (Player)
	{
		MoveToActor(Player, AcceptanceRadius);
		AI->AimAt(Player->GetActorLocation());
		AI->Fire();
	}
}