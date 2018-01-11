// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Mortar.generated.h"

UCLASS()
class BATTLETANK_API AMortar : public APawn
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeadDelegate);
	
public:	
	// Sets default values for this actor's properties
	AMortar();
	UFUNCTION(BlueprintPure, Category = "Health")
		float GetHealthPercent();
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
	FOnDeadDelegate OnDeath;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float MaxHealth = 100;
	UPROPERTY(VisibleAnywhere, Category = "Setup")
		float CurrentHealth = 100;
};
