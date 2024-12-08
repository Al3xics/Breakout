// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BKGameMode.generated.h"

class ABKGameBall;
class ABKPaddle;
/**
 * 
 */
UCLASS()
class BREAKOUT_API ABKGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	bool PerformBoxTrace(FHitResult& OutHit, const FVector& StartLocation, const FVector& EndLocation, const FVector& BoxExtent,
						 const FVector& BoxOffset, ETraceTypeQuery TraceChannel, float VisibilityDuration) const;

	UFUNCTION()
	void SpawnGameBall(const ABKPaddle* Paddle);
	
	UFUNCTION()
	void SnapToGround(AActor* Actor) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BK Game Mode")
	TSubclassOf<ABKGameBall> GameBallBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BK Game Mode")
	FVector GameBoxCenter = FVector(700.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BK Game Mode")
	FVector GameBoxExtent = FVector(800.0f, 600.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category="BK Game Mode")
	bool bShowBox = true;

	UPROPERTY()
	AActor* WallActor;

	UPROPERTY()
	ABKGameBall* GameBall;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
};
