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
	void SetParams(FString name, FVector scale, FVector rotation, float rotationModifier);
	FString GetPlanetName();
	FBoxSphereBounds GetBounds();
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
	FString mats[9] = { "/Game/Materials/0_Mat", "/Game/Materials/1_Mat", "/Game/Materials/2_Mat", "/Game/Materials/3_Mat", "/Game/Materials/4_Mat", "/Game/Materials/5_Mat", "/Game/Materials/6_Mat", "/Game/Materials/7_Mat", "/Game/Materials/8_Mat" };
};
