// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/Character.h"
#include "BKPaddle.generated.h"

class ABKGameMode;
class UBKBoundaryWallComponent;

UCLASS()
class BREAKOUT_API ABKPaddle : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABKPaddle();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="BK Paddle")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="BK Paddle")
	UBKBoundaryWallComponent* BoundaryWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BK Paddle")
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BK Paddle")
	UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BK Paddle")
	FVector BoxCenter = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BK Paddle")
	FVector BoxExtent = FVector(50.0f, 50.0f, 10.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BK Paddle")
	bool bShowBox = true;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private :
	UFUNCTION()
	void OnMoveTriggered(const FInputActionValue& Value);

	UPROPERTY()
	FVector CurrentLocation;

	UPROPERTY()
	ABKGameMode* BkGameMode;
};
