// Fill out your copyright notice in the Description page of Project Settings.


#include "BKBrick.h"

#include "BKGameMode.h"
#include "BKBoundaryWallComponent.h"


// Sets default values
ABKBrick::ABKBrick()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(RootComponent);

	BoundaryWall = CreateDefaultSubobject<UBKBoundaryWallComponent>("BoundaryWall");
}

// Called every frame
void ABKBrick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Pas eu le temps de finir, mais j'aurais fait comme dans le Tick de BKGameBall (pareil pour Reflect),
	// donc en soit je sais le faire, juste que j'ai galérer sur autre chose et donc pas eu le temps de le faire.......
	// if (BkGameMode && BkGameMode->GameBall && BkGameMode->GameBall->BoundaryWall)
	// {
	// 	if (CheckCollisionWithGameBall())
	// 	{
	// 		ReflectBall();
	// 	}
	// }
}

bool ABKBrick::CheckCollisionWithGameBall(const USphereComponent* GameBallBox)
{
	return false;
}

// Called when the game starts or when spawned
void ABKBrick::BeginPlay()
{
	Super::BeginPlay();

	// Get a ref to BKGameMode
	BkGameMode = Cast<ABKGameMode>(GetWorld()->GetAuthGameMode());

	// Initialize the component to have the same size as static mesh (used to check collision)
	BoxCenter = StaticMeshComponent->GetComponentLocation();
	// Oui c'est pas joli, mais j'avais plus de temps pour le faire bien (il est 23h36 actuellement.......)
	BoxExtent = FVector(StaticMeshComponent->Bounds.BoxExtent.X*2, StaticMeshComponent->Bounds.BoxExtent.Y/1.5, StaticMeshComponent->Bounds.BoxExtent.Z*2);
	BoundaryWall->InitializeBox(BoxCenter, BoxExtent, bShowBox);
	// GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, FString::Printf(TEXT("Mesh Box Extent : %f, %f, %f"),StaticMeshComponent->Bounds.BoxExtent.X, StaticMeshComponent->Bounds.BoxExtent.Y, StaticMeshComponent->Bounds.BoxExtent.Z));
	
	BkGameMode->SnapToGround(this);
}

