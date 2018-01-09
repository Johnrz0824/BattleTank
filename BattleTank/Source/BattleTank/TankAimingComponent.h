// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

class UTankBarrel;
class UTankTurret;
class AProjectile;

//Enum for 
UENUM()
enum class EFiringStatus : uint8
{
	Aiming,
	Reloading,
	Locked,
	OutOfAmmo
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();
	void SetBarrelReference(UTankBarrel* BarrelToSet);
	void SetTurretReference(UTankTurret* TurretToSet);
	void AimAt(FVector HitLocation);
	void UpdateState();
	EFiringStatus GetFiringState() const;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	UFUNCTION(BlueprintCallable, Category = "Firing")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Firing")
	int32 GetRemainAmmos();
private:
	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;
	FVector AimDirection;
	void MoveBarrelTowards(FVector Direction);
	bool IsBarrelMoving();

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 100000;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint;

protected:
	float LastFireTime = 0;
	void BeginPlay()override;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	EFiringStatus FiringState = EFiringStatus::Reloading;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ReloadTime = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 Ammos = 0;
};
