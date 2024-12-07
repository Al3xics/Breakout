// Fill out your copyright notice in the Description page of Project Settings.


#include "BKPaddle.h"

#include "BKBoundaryWallComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABKPaddle::ABKPaddle()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->GravityScale = 0.0f;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(RootComponent);

	BoundaryWall = CreateDefaultSubobject<UBKBoundaryWallComponent>("BoundaryWall");
	// BoundaryWall = NewObject<UBKBoundaryWallComponent>(this);
	// BoundaryWall->Initialize(BoxCenter, BoxExtent, bShowBox);
	
}

// Called every frame
void ABKPaddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	const float MinY = BkGameMode->GameBoxCenter.Y - BkGameMode->GameBoxExtent.Y;
	const float MaxY = BkGameMode->GameBoxCenter.Y + BkGameMode->GameBoxExtent.Y;
	
	if (GetActorLocation().Y < MinY || GetActorLocation().Y > MaxY)
	{
		// Clamp location of the paddle between the game box
		CurrentLocation = GetActorLocation();
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("Current Location : %f"), CurrentLocation.Y));
		
		CurrentLocation.Y = FMath::Clamp(CurrentLocation.Y, MinY, MaxY);
		SetActorLocation(CurrentLocation);
	}
}

// Called to bind functionality to input
void ABKPaddle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ABKPaddle::OnMoveTriggered);

}

// Called when the game starts or when spawned
void ABKPaddle::BeginPlay()
{
	Super::BeginPlay();

	// Get a ref to BKGameMode
	BkGameMode = Cast<ABKGameMode>(GetWorld()->GetAuthGameMode());

	// Initialize the component used to check collision
	BoundaryWall->Initialize(BoxCenter, BoxExtent, bShowBox);
	
	// Add mapping context
	if (const auto PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (!InputMappingContext.IsNull())
			{
				PlayerSubsystem->AddMappingContext(InputMappingContext.LoadSynchronous(), 0);
				// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, "InputMappingContext");
			}
		}
	}
	
}

void ABKPaddle::OnMoveTriggered(const FInputActionValue& Value)
{
	const float AxisValue = Value.Get<float>();
	
	AddMovementInput(FVector::RightVector, AxisValue);
	// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, "Move triggered");
	// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("%f"), AxisValue));
	
}

