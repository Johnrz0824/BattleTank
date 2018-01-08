// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"


UTankTrack::UTankTrack()
{
	
}

void UTankTrack::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("!@@!@"));
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}
void UTankTrack::SetThrottle(float Throttle)
{
	//auto Name = GetName();
	//UE_LOG(LogTemp, Warning, TEXT("%s throttle: %f"), *Name, Throttle);
	auto ForceApplied = GetForwardVector() * Throttle * TrackMaxDrivingForce;
	auto ForceLocation = GetComponentLocation();
	auto TankRoot =Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}

void UTankTrack::Slip(float DeltaTime)
{
	//Cal the sllippage speed
	auto SllippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
	//Work-out tje required acceleration this frame to correct
	auto CorrectionAcceletation = -SllippageSpeed / DeltaTime * GetRightVector();
	//Calcute and apply sidewaus for(F=ma)
	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());

	auto CorrectionForce = TankRoot->GetMass() * CorrectionAcceletation / 2; //2 tracks
	TankRoot->AddForce(CorrectionForce);
}

void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
}