// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "BKBoundaryWallComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BREAKOUT_API UBKBoundaryWallComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBKBoundaryWallComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, Category="BK Boundary Wall")
	FVector BoxCenter;
	
	UPROPERTY(EditAnywhere, Category="BK Boundary Wall")
	FVector BoxExtent;

	UPROPERTY(EditAnywhere, Category="BK Boundary Wall")
	bool bShowBox;
	
	UFUNCTION(Blueprintable)
	void Initialize(const FVector& NewCenter, const FVector& NewExtent, const bool bEnable);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category="BK Boundary Wall Component")
	UBoxComponent* WallBox;
};