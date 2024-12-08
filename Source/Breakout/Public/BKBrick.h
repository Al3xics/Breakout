// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BKBrick.generated.h"

class USphereComponent;
class ABKGameMode;
class UBKBoundaryWallComponent;

UCLASS()
class BREAKOUT_API ABKBrick : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABKBrick();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="BK Brick")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="BK Brick")
	UBKBoundaryWallComponent* BoundaryWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BK Brick")
	FVector BoxCenter = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BK Brick")
	FVector BoxExtent = FVector(50.0f, 50.0f, 10.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BK Brick")
	bool bShowBox = true;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Blueprintable)
	bool CheckCollisionWithGameBall(const USphereComponent* GameBallBox);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	FVector CurrentLocation;

	UPROPERTY()
	ABKGameMode* BkGameMode;
};
