// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VoyagerMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
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

};
