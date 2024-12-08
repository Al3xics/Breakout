// Fill out your copyright notice in the Description page of Project Settings.


#include "BKGameMode.h"

#include "BKBoundaryWallComponent.h"
#include "BKBrick.h"
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

void ABKGameMode::GenerateBricks()
{
	if (!BrickBlueprint)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Brick Blueprint assigned!"));
		return;
	}

	if (BrickMaterials.Num() < 4)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You need at least 4 materials assigned!"));
		return;
	}

	const FVector GameBoxMin = GameBoxCenter - GameBoxExtent;
    const FVector GameBoxMax = GameBoxCenter + GameBoxExtent;

    const int32 NumRows = Rows;
    const int32 NumColumns = Columns;

    // Dynamically calculate brick sizes to fit entire GameBox width/height
    const float TotalWidth = GameBoxMax.Y - GameBoxMin.Y;
    const float TotalHeight = GameBoxMax.X - GameBoxMin.X;

    const float BrickWidth = (TotalWidth - (NumColumns - 1) * BrickSpacing) / NumColumns;
    const float BrickHeight = (TotalHeight - (NumRows - 1) * BrickSpacing) / NumRows;

    // Starting position in the upper right corner of the GameBox
    const FVector StartLocation(
        GameBoxMax.X - BrickHeight / NumColumns - BrickSpacing, // Vertical offset from top
        GameBoxMax.Y - BrickWidth / 2.0f, // Horizontal shift to the right
        GameBoxCenter.Z + GameBoxExtent.Z
    );

    // Generation of bricks
    for (int32 Row = 0; Row < NumRows; ++Row)
    {
        for (int32 Col = 0; Col < NumColumns; ++Col)
        {
            // Calculate the position of each brick by going down line by line
            const FVector BrickLocation(
                StartLocation.X - Row * (BrickHeight / NumColumns + BrickSpacing),
                StartLocation.Y - Col * (BrickWidth + BrickSpacing),
                StartLocation.Z
            );

            // Spawn Brick
            FActorSpawnParameters SpawnParams;
            const ABKBrick* Brick = GetWorld()->SpawnActor<ABKBrick>(BrickBlueprint, BrickLocation, FRotator::ZeroRotator, SpawnParams);
        	
            if (Brick)
            {
                UStaticMeshComponent* BrickMesh = Brick->FindComponentByClass<UStaticMeshComponent>();
                if (BrickMesh)
                {
                    const int32 MaterialIndex = FMath::Clamp(Row / 2, 0, BrickMaterials.Num() - 1);
                    BrickMesh->SetMaterial(0, BrickMaterials[MaterialIndex]);
                }
            }
        }
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
		else if (ABKBrick* Brick = Cast<ABKBrick>(Actor))
		{
			Brick->SetActorLocation(FVector(StartLocation.X, StartLocation.Y, Brick->BoxExtent.Z + GroundLocation.Z));
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

	GenerateBricks();
}
