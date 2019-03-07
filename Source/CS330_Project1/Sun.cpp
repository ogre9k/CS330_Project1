// Fill out your copyright notice in the Description page of Project Settings.

#include "Sun.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASun::ASun()
{
	PlanetName = FString(TEXT("Sun"));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create and position a mesh component so we can see where our sphere is
	SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	RootComponent = SphereVisual;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));

	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
	}

	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;

	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("SpawnedSphere"));
	SphereVisual->SetupAttachment(RootComponent);

	SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
	// SphereVisual->SetWorldScale3D(FVector(4.0f));

	OurMovementComponent = CreateDefaultSubobject<UPlanetRotationMovement>(TEXT("CustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;
	//OurMovementComponent->PivotTranslation = FVector(200, 300, 0); Sun just spins, but if it did orbit this is how we'd do it
	
}

void ASun::SetParams(FString name, FVector scale, FVector rotation)
{
	PlanetName = name;
	SphereVisual->SetWorldScale3D(scale);
	OurMovementComponent->PivotTranslation = rotation;
}
// Called when the game starts or when spawned
void ASun::BeginPlay()
{
	Super::BeginPlay();
}
FString ASun::GetPlanetName()
{
	return PlanetName;
}
// Called every frame
void ASun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

