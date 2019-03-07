// Fill out your copyright notice in the Description page of Project Settings.

#include "Voyager.h"
#include "Engine.h"
#include "Sun.h"
#include "Components/SphereComponent.h"

// Sets default values
AVoyager::AVoyager()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BearingTime = 2.0f;
	Speed = 300.0f;
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
    
    distanceFactor = 5.0f;
}

// Called when the game starts or when spawned
void AVoyager::BeginPlay()
{
	Super::BeginPlay();
	FVector sunLocation = FVector(2340.0, -1360.0, 1460.0);
	if (GetWorld())
	{
		// Spawn the sun
		sun = GetWorld()->SpawnActor<ASun>(sunLocation, GetActorRotation());
		sun->SetParams(FString(TEXT("Sun")), FVector(4.0f), FVector(0.0f, 0.0f, 0.0f));
		FVector sunOrigin, boundingBox;
		sun->GetActorBounds(false, sunOrigin, boundingBox);
		sunRadius = boundingBox.Z / 2.0f;
		sun->SetPivotOffset(FVector(sunOrigin.X, sunOrigin.Y, sunOrigin.Z + sunRadius));
        planetNumber = 1;
	}
}

// Called every frame
void AVoyager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (rotating) {

		//Disable the inputs on this actor
		if (GetWorld())
		{
			APlayerController *myPlayerController = GetWorld()->GetFirstPlayerController();

			if (myPlayerController != NULL)
			{
				this->DisableInput(myPlayerController);
			}
		}

		// Rotate!
		RotateFactor += (DeltaTime / BearingTime);
		RotateFactor = FMath::Clamp<float>(RotateFactor, 0.0f, 1.0f);
		FRotator NewRotation = FMath::Lerp(initRotateDir.Rotation(), targetDir.Rotation(), RotateFactor);
		SpringArm->SetWorldRotation(NewRotation);

		// Zoom!
		// Move voyager halfway to Sun
		FVector sunLocation = sun->GetActorLocation();
		FVector distanceVector = sunLocation - initialLocation;
		FVector NewLocation = FMath::Lerp(initialLocation, sunLocation, RotateFactor/2);
		FVector vectorToNewLocation = NewLocation - GetActorLocation();
		OurMovementComponent->AddInputVector(vectorToNewLocation);

		// Check if camera is looking at the object
		if (RotateFactor >= 1.0f)
		{
			RotateFactor = 0;
			rotating = false;
			// Re-enable inputs
			if (GetWorld())
			{
				APlayerController *myPlayerController = GetWorld()->GetFirstPlayerController();

				if (myPlayerController != NULL)
				{
					this->EnableInput(myPlayerController);
				}
				if (GEngine)
				{
					float distX, distY, distZ;
					distX = sun->GetActorLocation().X - GetActorLocation().X;
					distY = sun->GetActorLocation().Y - GetActorLocation().Y;
					distZ = sun->GetActorLocation().Z + - GetActorLocation().Z;
					float totalDistance = sqrt((distX*distX + distY * distY + distZ * distZ));
					GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Bearing mode finished: Distance is : %f"), totalDistance));
				}
			}
		}
	}
	else {

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
}

// Called to bind functionality to input
void AVoyager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveForward", this, &AVoyager::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AVoyager::MoveRight);
	InputComponent->BindAxis("Turn", this, &AVoyager::Turn);
	InputComponent->BindAxis("TurnUp", this, &AVoyager::TurnUp);
	InputComponent->BindAction("RightClick", IE_Pressed, this, &AVoyager::RightClick);
    InputComponent->BindAction("Spawner", IE_Pressed, this, &AVoyager::Spawner);
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

void AVoyager::RightClick() 
{
	rotating = true;
	initRotateDir = SpringArm->GetForwardVector();
	initRotateDir.Normalize();
	targetDir = sun->GetPivotOffset() - SpringArm->GetComponentLocation();
	targetDir.Normalize();
	initialLocation = GetActorLocation();

	float distX, distY, distZ;
	distX = sun->GetActorLocation().X - initialLocation.X;
	distY = sun->GetActorLocation().Y - initialLocation.Y;
	distZ = sun->GetActorLocation().Z - initialLocation.Z;
	float totalDistance = sqrt((distX*distX + distY * distY + distZ * distZ));

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Bearing mode activated: Distance is : %f"), totalDistance));
	}
}

// Make this a for-loop, not a Switch statement.
// Variables here are scale, planet name (if it matters), distance from the sun.
// Other variables, that belong in the other class, are the texture that will be used.
// This can be implemented in SetParams.
void AVoyager::Spawner()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Green, FString::Printf(TEXT("PlanetNumber: %d"), planetNumber));
    }
    switch(planetNumber)
    {
        case 1: // Scale = 0.5, Dist = 100
        {
            FVector offset = FVector(100.0f * distanceFactor, 0.0f, 0.0f);
            planets[planetNumber - 1] = GetWorld()->SpawnActor<ASun>(sun->GetActorLocation() + offset, GetActorRotation());
            planets[planetNumber - 1]->SetParams(FString(TEXT("Planet X")), FVector(0.5f), FVector(0.0f, 0.0f, 0.0f));
            planets[planetNumber - 1]->GetActorBounds(false, planetOrigin, boundingBox);
            radius = boundingBox.Z / 2.0f;
            planets[planetNumber - 1]->SetPivotOffset(FVector(planetOrigin.X, planetOrigin.Y, planetOrigin.Z + radius));
            planetNumber++;
            break;
        }
        case 2: // Scale = 1, Dist = 150
        {
            planets[planetNumber - 1] = GetWorld()->SpawnActor<ASun>(sun->GetActorLocation(), GetActorRotation());
            planets[planetNumber - 1]->SetParams(FString(TEXT("Planet X")), FVector(0.5f), FVector(0.0f, 0.0f, 0.0f));
            planets[planetNumber - 1]->GetActorBounds(false, planetOrigin, boundingBox);
            radius = boundingBox.Z / 2.0f;
            planets[planetNumber - 1]->SetPivotOffset(FVector(planetOrigin.X, planetOrigin.Y, planetOrigin.Z + radius));
            planetNumber++;
            break;
        }
        case 3: // Scale = 1, Dist = 200
        {
            planets[planetNumber - 1] = GetWorld()->SpawnActor<ASun>(sun->GetActorLocation(), GetActorRotation());
            planets[planetNumber - 1]->SetParams(FString(TEXT("Planet X")), FVector(0.5f), FVector(0.0f, 0.0f, 0.0f));
            planets[planetNumber - 1]->GetActorBounds(false, planetOrigin, boundingBox);
            radius = boundingBox.Z / 2.0f;
            planets[planetNumber - 1]->SetPivotOffset(FVector(planetOrigin.X, planetOrigin.Y, planetOrigin.Z + radius));
            planetNumber++;
            break;
        }
        case 4: // Scale = 1, Dist = 300
        {
            planets[planetNumber - 1] = GetWorld()->SpawnActor<ASun>(sun->GetActorLocation(), GetActorRotation());
            planets[planetNumber - 1]->SetParams(FString(TEXT("Planet X")), FVector(0.5f), FVector(0.0f, 0.0f, 0.0f));
            planets[planetNumber - 1]->GetActorBounds(false, planetOrigin, boundingBox);
            radius = boundingBox.Z / 2.0f;
            planets[planetNumber - 1]->SetPivotOffset(FVector(planetOrigin.X, planetOrigin.Y, planetOrigin.Z + radius));
            planetNumber++;
            break;
        }
        case 5: // Scale = 2.5, Dist = 450
            planetNumber++;
            break;
        case 6: // Scale = 2.5, Dist = 600
            planetNumber++;
            break;
        case 7: // Scale = 1.5, Dist = 800
            planetNumber++;
            break;
        case 8: // Scale = 1.5, Dist = 1000
            planetNumber++;
            break;
        default:
            // Do nothing
            break;
    }
}

float AVoyager::GetSpeed()
{
	return Speed;
}
