// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"
UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	ATank();
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
protected:
	UPROPERTY(EditDefaultsOnly, Category ="Setup")
	float MaxHealth = 100;
	UPROPERTY(VisibleAnywhere, Category = "Setup")
	float CurrentHealth = 100;
};
