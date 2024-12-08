// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BKBrick.generated.h"

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

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
};
