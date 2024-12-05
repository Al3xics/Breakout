// Fill out your copyright notice in the Description page of Project Settings.


#include "BKGameBall.h"

// Sets default values
ABKGameBall::ABKGameBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABKGameBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABKGameBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

