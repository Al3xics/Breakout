// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BKGameMode.generated.h"

class ABKBrick;
class UBKBoundaryWallComponent;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BK Game Mode")
	TSubclassOf<ABKGameBall> GameBallBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BK Game Mode")
	TSubclassOf<ABKBrick> BrickBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BK Game Mode")
	TArray<UMaterialInterface*> BrickMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BK Game Mode")
	FVector GameBoxCenter = FVector(700.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BK Game Mode")
	FVector GameBoxExtent = FVector(800.0f, 600.0f, 300.0f);

	UPROPERTY(EditAnywhere, Category="BK Game Mode")
	bool bShowBox = true;

	UPROPERTY(EditAnywhere, Category="BK Game Mode")
	int32 Rows = 8;

	UPROPERTY(EditAnywhere, Category="BK Game Mode")
	int32 Columns = 8;

	UPROPERTY(EditAnywhere, Category="BK Game Mode")
	float BrickSpacing = 50.0f;

	UPROPERTY()
	AActor* WallActor;

	UPROPERTY()
	UBKBoundaryWallComponent* WallComponent;

	UPROPERTY()
	ABKGameBall* GameBall;

	UPROPERTY()
	ABKPaddle* BkPaddle;
	
	UFUNCTION()
	void SpawnGameBall(const ABKPaddle* Paddle);

	UFUNCTION()
	void GenerateBricks();
	
	UFUNCTION()
	void SnapToGround(AActor* Actor) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
};
