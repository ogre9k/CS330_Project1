// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VoyagerMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Sun.h"
#include "Voyager.generated.h"

UCLASS()
class CS330_PROJECT1_API AVoyager : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVoyager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class UVoyagerMovementComponent* OurMovementComponent;
	USpringArmComponent* SpringArm;
	FVector2D CameraInput;
    ASun *sun;
    ASun *planets[8];

	UPROPERTY(EditAnywhere)
	float BearingTime;
	UPROPERTY(EditAnywhere)
	float Speed;

	float RotateFactor;
	bool rotating;
	// Variable that holds the initial direction to turn from
	FVector initRotateDir;
	FVector targetDir;
	FVector initialLocation;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Turn(float AxisValue);
	void TurnUp(float AxisValue);
	void RightClick();
    void Spawner();
	float GetSpeed();
    float distanceFactor;
	float scales[8] = { 0.5f, 1.0f, 1.0f, 1.0f, 2.5f, 2.5f, 1.5f, 2.5f };
	float relativeDistance[8] = { 100.0f, 150.0f, 200.0f, 300.0f, 450.0f, 600.0f, 800.0f, 1000.0f };
    int planetNumber;
};
