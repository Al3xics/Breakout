// Fill out your copyright notice in the Description page of Project Settings.


#include "BKBoundaryWallComponent.h"

#include "BKGameBall.h"
#include "BKPaddle.h"


// Sets default values for this component's properties
UBKBoundaryWallComponent::UBKBoundaryWallComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Verify the type of owner
	if (const AActor* Owner = GetOwner())
	{
		if (Owner->IsA(ABKPaddle::StaticClass()))
		{ 
			WallBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WallBox"));
		}
		else if (Owner->IsA(ABKGameBall::StaticClass()))
		{
			WallSphere = CreateDefaultSubobject<USphereComponent>(TEXT("WallSphere"));
		}
		else if (Owner->IsA(AActor::StaticClass())) // Use when spawning an AActor (so it doesn't correspond to any other class)
		{
			WallBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WallBox"));
		}
	}
	
}


// Called when the game starts
void UBKBoundaryWallComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!IsRegistered()) RegisterComponent();
	
	// Verify the type of owner
	if (const AActor* Owner = GetOwner())
	{
		if (Owner->IsA(ABKPaddle::StaticClass()))
		{
			WallBox->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			WallBox->SetWorldLocation(BoundaryCenter);
			WallBox->SetBoxExtent(BoundaryExtent);
			WallBox->SetHiddenInGame(!bShowBoundary);
			
		}
		else if (Owner->IsA(ABKGameBall::StaticClass()))
		{
			WallSphere->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			WallSphere->SetWorldLocation(BoundaryCenter);
			WallSphere->SetSphereRadius(SphereRadius);
			WallSphere->SetHiddenInGame(!bShowBoundary);
		}
		else if (Owner->IsA(AActor::StaticClass()))
		{
			WallBox->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			WallBox->SetWorldLocation(BoundaryCenter);
			WallBox->SetBoxExtent(BoundaryExtent);
			WallBox->SetHiddenInGame(!bShowBoundary);
		}
	}
}


// Called every frame
void UBKBoundaryWallComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UBKBoundaryWallComponent::InitializeBox(const FVector& NewCenter, const FVector& NewExtent, const bool bEnable)
{
	BoundaryCenter = NewCenter;
	BoundaryExtent = NewExtent;
	bShowBoundary = bEnable;
	
	WallBox->SetWorldLocation(BoundaryCenter);
	WallBox->SetBoxExtent(BoundaryExtent);
	WallBox->SetHiddenInGame(!bShowBoundary);

	if (!WallBox->IsRegistered()) WallBox->RegisterComponent(); // Ensure WallBox is registered
}

void UBKBoundaryWallComponent::InitializeSphere(const FVector& NewCenter, const float NewSphereRadius, const bool bEnable)
{
	BoundaryCenter = NewCenter;
	SphereRadius = NewSphereRadius;
	bShowBoundary = bEnable;
	
	WallSphere->SetWorldLocation(BoundaryCenter);
	WallSphere->SetSphereRadius(SphereRadius);
	WallSphere->SetHiddenInGame(!bShowBoundary);

	if (!WallSphere->IsRegistered()) WallSphere->RegisterComponent(); // Ensure WallSphere is registered
}

