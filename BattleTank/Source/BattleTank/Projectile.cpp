// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Movement Component"));
	MovementComponent->bAutoActivate = false;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	SetRootComponent(CollisionMesh);
	CollisionMesh->SetNotifyRigidBodyCollision(true);
	
	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	LaunchBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	ImpactBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactBlast->bAutoActivate = false;

	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion Force"));
	ExplosionForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//ExplosionForce->bIgnoreOwningActor = true;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::LaunchProjectile(float Speed)
{
	MovementComponent->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	MovementComponent->Activate();
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	LaunchBlast->Deactivate();
	ImpactBlast->Activate();
	ExplosionForce->FireImpulse();
	
	SetRootComponent(ImpactBlast);
	CollisionMesh->DestroyComponent();

	FTimerHandle handle;
	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(handle, this, &AProjectile::DestroyOnExpire, DestroyDelay,false);
}

void AProjectile::DestroyOnExpire()
{
	this->Destroy();
}