// Fill out your copyright notice in the Description page of Project Settings.


#include "BKGameMode.h"

#include "BKBoundaryWallComponent.h"
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

void ABKGameMode::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnsParams;
	SpawnsParams.Owner = this;
	
	WallActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), GameBoxCenter, FRotator::ZeroRotator, SpawnsParams);
	if (WallActor)
	{
		if (UBKBoundaryWallComponent* WallComponent = NewObject<UBKBoundaryWallComponent>(WallActor))
		{
			WallComponent->Initialize(GameBoxCenter, GameBoxExtent, bShowBox);
		}
	}

	// if (bShowBox)
	// {
	// 	DrawDebugBox(GetWorld(), GameBoxCenter, GameBoxExtent, FColor::Purple, true, -1, 0, 5);
	// }
}
