// Fill out your copyright notice in the Description page of Project Settings.


#include "BKGameMode.h"

#include "BKBoundaryWallComponent.h"
#include "BKGameBall.h"
#include "BKPaddle.h"
#include "EngineUtils.h"

void ABKGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ABKGameMode::SpawnGameBall(const ABKPaddle* Paddle)
{
	if (!Paddle) return;

	const FVector PaddleLocation = Paddle->GetActorLocation();
	// Used to have the game ball spawned above the paddle (X), between the  game box (Y), en above the paddle (Z, will be snap to ground after)<
	const FVector SpawnLocation = PaddleLocation + FVector(FMath::RandRange(PaddleLocation.X + Paddle->BoxExtent.X * 10, GameBoxExtent.X),
														   FMath::RandRange(-GameBoxExtent.Y, GameBoxExtent.Y),
														   FMath::RandRange(PaddleLocation.Z + Paddle->BoxExtent.Z * 2, PaddleLocation.Z + Paddle->BoxExtent.Z * 2));

	const FActorSpawnParameters SpawnParams;
	GameBall = GetWorld()->SpawnActor<ABKGameBall>(GameBallBlueprint, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	if (GameBall)
	{
		GameBall->InitializeGameBall(PaddleLocation, GameBoxCenter, GameBoxExtent);
	}
}

void ABKGameMode::SnapToGround(AActor* Actor) const
{
	const FVector StartLocation = Actor->GetActorLocation();

	// Trace line toward bottom
	const FVector EndLocation = StartLocation - FVector(0.0f, 0.0f, 1000.0f);
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Actor); // Ignore himself

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		QueryParams
	);

	if (bHit)
	{
		// Move actor to ground
		const FVector GroundLocation = HitResult.Location;
		if (ABKPaddle* Paddle = Cast<ABKPaddle>(Actor))
		{
			Paddle->SetActorLocation(FVector(StartLocation.X, StartLocation.Y, Paddle->BoxExtent.Z + GroundLocation.Z));
		}
		else if (ABKGameBall* Ball = Cast<ABKGameBall>(Actor))
		{
			Ball->SetActorLocation(FVector(StartLocation.X, StartLocation.Y, Ball->SphereRadius/2 + GroundLocation.Z));
		}

		
	}
}

void ABKGameMode::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ABKPaddle> It(GetWorld()); It; ++It)
	{
		BkPaddle = *It;
	}

	FActorSpawnParameters SpawnsParams;
	SpawnsParams.Owner = this;
	
	WallActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), GameBoxCenter, FRotator::ZeroRotator, SpawnsParams);
	if (WallActor)
	{
		WallComponent = NewObject<UBKBoundaryWallComponent>(WallActor);
		if (WallComponent)
		{
			if (!WallComponent->IsRegistered()) WallComponent->RegisterComponent();
			
			// GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("Wall Actor Owner : %s"), *WallActor->GetOwner()->GetName()));
			// GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("Wall Component Owner : %s"), *WallComponent->GetOwner()->GetName()));
			WallComponent->InitializeBox(GameBoxCenter, GameBoxExtent, bShowBox);
		}
	}
}
