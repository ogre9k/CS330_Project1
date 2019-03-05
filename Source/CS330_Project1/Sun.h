// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlanetRotationMovement.h"
#include "Sun.generated.h"

UCLASS()
class CS330_PROJECT1_API ASun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	class UPlanetRotationMovement* OurMovementComponent;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};