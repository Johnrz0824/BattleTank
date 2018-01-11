// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "GameFramework/Pawn.h"

ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = false;
}

float ATank::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	float DamageToApply = FMath::Clamp<int32>(DamagePoints, 0, CurrentHealth);
	UE_LOG(LogTemp, Warning, TEXT("%f"), CurrentHealth);
	CurrentHealth -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("%f"), CurrentHealth);
	if(CurrentHealth<=0)
		OnDeath.Broadcast();
	return DamageToApply;
}

float ATank::GetHealthPercent() 
{
	return CurrentHealth / MaxHealth;
}