// Fill out your copyright notice in the Description page of Project Settings.

#include "Sun.h"
#include "Engine.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

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

	OurMovementComponent = CreateDefaultSubobject<UPlanetRotationMovement>(TEXT("CustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;

	// Find the Voyager Object (there should only be 1: the player)
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASun::StaticClass(), actors);
	int index = actors.Num() - 1;

	ConstructorHelpers::FObjectFinder<UMaterial> material(*mats[index]);
	if (material.Succeeded())
	{
		SphereVisual->SetMaterial(0, material.Object);
	}

	// If this is the first "Sun" object to spawn, treat it as the sun and ignite it.
	if (index == 0)
	{
		FBoxSphereBounds bounds = GetBounds();
		float radius = bounds.SphereRadius * 4.0f;
		SetPivotOffset(FVector(bounds.Origin.X, bounds.Origin.Y, bounds.Origin.Z));
		OurParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
		OurParticleSystem->SetupAttachment(SphereVisual);
		OurParticleSystem->SetRelativeLocation(GetPivotOffset());
		static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
		if (ParticleAsset.Succeeded())
		{
			OurParticleSystem->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
			OurParticleSystem->SetTemplate(ParticleAsset.Object);
		}
	}
}

void ASun::SetParams(FString name, FVector scale, FVector rotation, float rotationModifier)
{
	PlanetName = name;
	SphereVisual->SetWorldScale3D(scale);
	OurMovementComponent->PivotTranslation = rotation;
	OurMovementComponent->RotationRate.SetComponentForAxis(EAxis::Type::Z, 30.0f / rotationModifier);
	FRotator temp = OurMovementComponent->RotationRate;
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

FBoxSphereBounds ASun::GetBounds()
{
	FTransform temp;
	return SphereVisual->CalcBounds(temp);
}

// Called every frame
void ASun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

