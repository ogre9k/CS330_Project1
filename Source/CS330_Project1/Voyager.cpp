// Fill out your copyright notice in the Description page of Project Settings.

#include "Voyager.h"
#include "Engine.h"
#include "Components/SphereComponent.h"

// Sets default values
AVoyager::AVoyager()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Our root component will be a sphere that reacts to physics
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	// Use a spring arm to give the camera smooth, natural-feeling motion.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 0.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;
	// Create a camera and attach to our spring arm
	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// Create an instance of our movement component, and tell it to update the root.
	OurMovementComponent = CreateDefaultSubobject<UVoyagerMovementComponent>(TEXT("CustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;

	// Take control of the default player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AVoyager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVoyager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Rotate our camera's yaw
	{
		FRotator NewRotation = SpringArm->GetComponentRotation();
		NewRotation.Yaw += CameraInput.X;
		SpringArm->SetWorldRotation(NewRotation);
	}

	//Rotate our camera's pitch, but limit it so we're always looking downward
	//NOTE: Camera's pitch is changing independent of actor's pitch
	{
		FRotator NewRotation = SpringArm->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, 80.0f);
		SpringArm->SetWorldRotation(NewRotation);
	}

}

// Called to bind functionality to input
void AVoyager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveForward", this, &AVoyager::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AVoyager::MoveRight);
	InputComponent->BindAxis("Turn", this, &AVoyager::Turn);
	InputComponent->BindAxis("TurnUp", this, &AVoyager::TurnUp);

}

UPawnMovementComponent* AVoyager::GetMovementComponent() const
{
	return OurMovementComponent;
}

void AVoyager::MoveForward(float AxisValue)
{
	if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
	{
		OurMovementComponent->AddInputVector(SpringArm->GetForwardVector()* AxisValue);
	}
}

void AVoyager::MoveRight(float AxisValue)
{
	if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
	{
		OurMovementComponent->AddInputVector(SpringArm->GetRightVector() * AxisValue);
	}
}

void AVoyager::Turn(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void AVoyager::TurnUp(float AxisValue)
{
	CameraInput.Y = AxisValue;
}