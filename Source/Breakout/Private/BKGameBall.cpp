// Fill out your copyright notice in the Description page of Project Settings.


#include "BKGameBall.h"

#include "BKBoundaryWallComponent.h"
#include "BKGameMode.h"

// Sets default values
ABKGameBall::ABKGameBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(RootComponent);

	BoundaryWall = CreateDefaultSubobject<UBKBoundaryWallComponent>("BoundaryWall");

}

// Called every frame
void ABKGameBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveBall(DeltaTime);

}

// Called when the game starts or when spawned
void ABKGameBall::BeginPlay()
{
	Super::BeginPlay();

	// Get a ref to BKGameMode
	BkGameMode = Cast<ABKGameMode>(GetWorld()->GetAuthGameMode());

	// Initialize the component to have the same size as static mesh (used to check collision)
	SphereCenter = StaticMeshComponent->GetComponentLocation();
	SphereRadius = StaticMeshComponent->Bounds.SphereRadius * 2;
	BoundaryWall->InitializeSphere(SphereCenter, SphereRadius, bShowSphere);
	
	BkGameMode->SnapToGround(this);
	
}

void ABKGameBall::InitializeGameBall(const FVector& PaddleLocation, const FVector& GameBoxCenter, const FVector& GameBoxExtent)
{
	CurrentLocation = GetActorLocation();

	// Compute initial direction : from the ball's current position to the paddle's position
	CurrentDirection = (PaddleLocation - CurrentLocation).GetSafeNormal();

	// Add a small random offset to the direction to avoid a perfect perpendicular trajectory
	const FVector RandomOffset = FVector(FMath::RandRange(-0.2f, 0.2f), FMath::RandRange(-0.2f, 0.2f), 0.0f);
	CurrentDirection += RandomOffset;
	CurrentDirection = CurrentDirection.GetSafeNormal();
}

void ABKGameBall::MoveBall(float DeltaTime)
{
	CurrentLocation = GetActorLocation();
	SetActorLocation(CurrentLocation + CurrentDirection * Velocity * DeltaTime);
}

