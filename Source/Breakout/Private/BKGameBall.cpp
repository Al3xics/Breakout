// Fill out your copyright notice in the Description page of Project Settings.


#include "BKGameBall.h"

#include "BKBoundaryWallComponent.h"
#include "BKGameMode.h"
#include "BKPaddle.h"

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

	if (BkGameMode && BkGameMode->GameBall && BkGameMode->GameBall->BoundaryWall)
	{
		if (CheckCollisionWithPaddle(BkPaddleBox))
		{
			ReflectBall(BkPaddleBox);
		}
		else if (CheckCollisionWithGameBox())
		{
			ReflectBall(BkGameMode->WallComponent->WallBox);
		}
	}

}

// Called when the game starts or when spawned
void ABKGameBall::BeginPlay()
{
	Super::BeginPlay();

	// Get a ref to BKGameMode
	BkGameMode = Cast<ABKGameMode>(GetWorld()->GetAuthGameMode());

	// Get a ref to BKPaddle and its WallBox
	BkPaddle = BkGameMode->BkPaddle;
	BkPaddleBox = BkGameMode->BkPaddle->BoundaryWall->WallBox;

	// Initialize the component to have the same size as static mesh (used to check collision)
	SphereCenter = StaticMeshComponent->GetComponentLocation();
	SphereRadius = StaticMeshComponent->Bounds.SphereRadius * 2;
	BoundaryWall->InitializeSphere(SphereCenter, SphereRadius, bShowSphere);
	
	BkGameMode->SnapToGround(this);
	
}

void ABKGameBall::InitializeGameBall(const FVector& PaddleLocation, const FVector& GameBoxCenter, const FVector& GameBoxExtent)
{
	// Compute initial direction : from the ball's current position to the paddle's position
	CurrentDirection = PaddleLocation - GetActorLocation();
	CurrentDirection.Z = 0.0f;
	CurrentDirection = CurrentDirection.GetSafeNormal();

	// Add a small random offset to the direction to avoid a perfect perpendicular trajectory
	const FVector RandomOffset = FVector(FMath::RandRange(-0.2f, 0.2f), FMath::RandRange(-0.2f, 0.2f), 0.0f);
	CurrentDirection += RandomOffset;
	CurrentDirection.Z = 0.0f;
	CurrentDirection = CurrentDirection.GetSafeNormal();
}

bool ABKGameBall::CheckCollisionWithPaddle(const UBoxComponent* PaddleBox)
{
	if (!BoundaryWall || !StaticMeshComponent)
	{
		return false;
	}
	
	const FVector BallPosition = GetActorLocation();
	const FVector PaddleLocation = PaddleBox->GetComponentLocation();
	const FVector PaddleExtent = PaddleBox->GetUnscaledBoxExtent();
	
	// Verify if ball location is in paddle box
	if (FVector::Dist(BallPosition, PaddleLocation) < PaddleExtent.Size())
	{
		// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Collision Detected"));
		return true;
	}
	
	return false;
}

bool ABKGameBall::CheckCollisionWithGameBox()
{
	if (!BoundaryWall || !StaticMeshComponent)
	{
		return false;
	}
	
	const FVector BallPosition = GetActorLocation();
    
	// Determine the limits of the GameBox
	const FVector GameBoxMin = BkGameMode->GameBoxCenter - BkGameMode->GameBoxExtent;
	const FVector GameBoxMax = BkGameMode->GameBoxCenter + BkGameMode->GameBoxExtent;

	// Check if the ball is outside the GameBox boundaries
	if (BallPosition.X - SphereRadius < GameBoxMin.X || BallPosition.X + SphereRadius > GameBoxMax.X ||
		BallPosition.Y - SphereRadius < GameBoxMin.Y || BallPosition.Y + SphereRadius > GameBoxMax.Y ||
		BallPosition.Z - SphereRadius < GameBoxMin.Z || BallPosition.Z + SphereRadius > GameBoxMax.Z)
	{
		DrawDebugBox(
			GetWorld(),
			BkGameMode->GameBoxCenter,
			BkGameMode->GameBoxExtent,
			FColor::Blue,
			false,
			1.0f
		);

		// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Balle hors de la GameBox"));
		return true;
	}

	return false;
}

void ABKGameBall::ReflectBall(const UBoxComponent* Box)
{
	if (!Box)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Box Found"));
		return;
	}

	// Retrieve the position of the ball and the center of the box
	const FVector BallPosition = GetActorLocation();
	const FVector BoxCenter = Box->GetComponentLocation();
	const FVector BoxExtent = Box->GetUnscaledBoxExtent();

	// Check if the collision is with a side or upper/lower wall
	bool bIsWallCollision = FMath::Abs(BallPosition.X - BoxCenter.X) > BoxExtent.X - SphereRadius ||
							FMath::Abs(BallPosition.Y - BoxCenter.Y) > BoxExtent.Y - SphereRadius;

	FVector ReflectedVelocity = CurrentDirection * Velocity;

	if (bIsWallCollision)
	{
		// Collision with a wall: Invert Y or X axis
		if (FMath::Abs(BallPosition.X - BoxCenter.X) > BoxExtent.X - SphereRadius)
		{
			ReflectedVelocity.X *= -1; // Bounce on an upper/lower wall
		}
		if (FMath::Abs(BallPosition.Y - BoxCenter.Y) > BoxExtent.Y - SphereRadius)
		{
			ReflectedVelocity.Y *= -1; // Bounce off a side wall
		}
	}
	else
	{
		// Collision with the paddle: Change the angle depending on the position on the paddle
		FVector PaddleNormal = (BallPosition - BoxCenter);
		PaddleNormal.Z = 0.0f; // Ignorer Z
		PaddleNormal = PaddleNormal.GetSafeNormal();

		// Apply normal reflection
		ReflectedVelocity = ReflectedVelocity - 2 * FVector::DotProduct(ReflectedVelocity, PaddleNormal) * PaddleNormal;

		// Always point the ball upwards in an X shape
		ReflectedVelocity.X = FMath::Abs(ReflectedVelocity.X);
	}

	// Force Z to 0 (no vertical movement)
	ReflectedVelocity.Z = 0.0f;

	// Update ball direction
	CurrentDirection = ReflectedVelocity.GetSafeNormal();

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Ball Reflected"));
}

void ABKGameBall::MoveBall(float DeltaTime)
{
	SetActorLocation(GetActorLocation() + CurrentDirection * Velocity * DeltaTime);
}

