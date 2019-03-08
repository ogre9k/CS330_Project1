// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlanetRotationMovement.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sun.generated.h"

UCLASS()
class CS330_PROJECT1_API ASun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASun();
	void SetParams(FString name, FVector scale, FVector rotation);
	FString GetPlanetName();
	FBoxSphereBounds GetBounds();
	void Ignite();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	class UPlanetRotationMovement* OurMovementComponent;
	UStaticMeshComponent* SphereVisual;
	UParticleSystemComponent* OurParticleSystem;

	FString PlanetName;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
