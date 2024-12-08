// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BKGameBall.generated.h"

class ABKGameMode;
class UBKBoundaryWallComponent;

UCLASS()
class BREAKOUT_API ABKGameBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABKGameBall();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="BK Game Ball")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="BK Game Ball")
	UBKBoundaryWallComponent* BoundaryWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BK Game Ball")
	FVector SphereCenter = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BK Game Ball")
	float SphereRadius = 70.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BK Game Ball")
	bool bShowSphere = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BK Game Ball")
	float Velocity = 600.f;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Blueprintable)
	void InitializeGameBall(const FVector& PaddleLocation, const FVector& GameBoxCenter, const FVector& GameBoxExtent);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UPROPERTY()
	FVector CurrentLocation;
	
	UPROPERTY()
	FVector CurrentDirection;

	UPROPERTY()
	ABKGameMode* BkGameMode;

	UFUNCTION()
	void MoveBall(float DeltaTime);
};
