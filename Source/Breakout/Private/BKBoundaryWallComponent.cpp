// Fill out your copyright notice in the Description page of Project Settings.


#include "BKBoundaryWallComponent.h"

#include "BKPaddle.h"


// Sets default values for this component's properties
UBKBoundaryWallComponent::UBKBoundaryWallComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	WallBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WallBox"));
	
}


// Called when the game starts
void UBKBoundaryWallComponent::BeginPlay()
{
	Super::BeginPlay();
	
	WallBox->RegisterComponent();
	WallBox->SetWorldLocation(BoxCenter);
	WallBox->SetBoxExtent(BoxExtent);
	WallBox->SetHiddenInGame(!bShowBox);
}


// Called every frame
void UBKBoundaryWallComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// The wall box will follow the paddle (used to check collisions)
	if (const ABKPaddle* Owner = Cast<ABKPaddle>(GetOwner()))
	{
		// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, "Wall Paddle");
		const FVector NewLocation = Owner->GetActorLocation() + BoxCenter;
		WallBox->SetWorldLocation(NewLocation);
	}
	
}

void UBKBoundaryWallComponent::Initialize(const FVector& NewCenter, const FVector& NewExtent, const bool bEnable)
{
	BoxCenter = NewCenter;
	BoxExtent = NewExtent;
	bShowBox = bEnable;
	
	WallBox->RegisterComponent();
	WallBox->SetWorldLocation(BoxCenter);
	WallBox->SetBoxExtent(BoxExtent);
	WallBox->SetHiddenInGame(!bShowBox);
}

