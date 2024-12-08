// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "BKBoundaryWallComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BREAKOUT_API UBKBoundaryWallComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBKBoundaryWallComponent();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="BK Boundary Wall")
	UBoxComponent* WallBox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="BK Boundary Wall")
	USphereComponent* WallSphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BK Boundary Wall")
	FVector BoundaryCenter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BK Boundary Wall")
	FVector BoundaryExtent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BK Boundary Wall")
	float SphereRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BK Boundary Wall")
	bool bShowBoundary;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(Blueprintable)
	void InitializeBox(const FVector& NewCenter, const FVector& NewExtent, const bool bEnable);
	
	UFUNCTION(Blueprintable)
	void InitializeSphere(const FVector& NewCenter, const float NewSphereRadius, const bool bEnable);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	
};
