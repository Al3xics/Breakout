// Fill out your copyright notice in the Description page of Project Settings.


#include "BKGameMode.h"

#include "BKBoundaryWallComponent.h"
#include "BKGameBall.h"
#include "BKPaddle.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h"

void ABKGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

bool ABKGameMode::PerformBoxTrace(FHitResult& OutHit, const FVector& StartLocation, const FVector& EndLocation, const FVector& BoxExtent,
                                  const FVector& BoxOffset, const ETraceTypeQuery TraceChannel, const float VisibilityDuration) const
{
	const FVector BoxCenter = StartLocation + BoxExtent;
	const FQuat BoxRotation = FQuat::Identity;
	const FLinearColor TraceColor = FLinearColor::Red;
	const FLinearColor HitColor = FLinearColor::Green;
	
	return UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(),
		BoxCenter,
		EndLocation,
		BoxExtent,
		static_cast<FRotator>(BoxRotation),
		TraceChannel,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::ForDuration,
		OutHit,
		true,
		TraceColor,
		HitColor,
		VisibilityDuration);
}

void ABKGameMode::SpawnGameBall(const ABKPaddle* Paddle)
{
	if (!Paddle) return;

	const FVector PaddleLocation = Paddle->GetActorLocation();
	// Used to have the game ball spawned above the paddle (X), between the  game box (Y), en above the paddle (Z, will be snaped to ground after)
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

	// if (bShowBoundary)
	// {
	// 	DrawDebugBox(GetWorld(), GameBoxCenter, GameBoxExtent, FColor::Purple, true, -1, 0, 5);
	// }
}
